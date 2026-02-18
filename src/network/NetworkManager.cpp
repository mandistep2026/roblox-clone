#include "NetworkManager.hpp"
#include "core/Logger.hpp"

namespace roblox_clone::network {

NetworkManager::NetworkManager() = default;

NetworkManager::~NetworkManager() {
    shutdown();
}

bool NetworkManager::initialize() {
    if (enet_initialize() != 0) {
        RC_ERROR("Failed to initialize ENet");
        return false;
    }
    RC_INFO("Network manager initialized");
    return true;
}

void NetworkManager::shutdown() {
    cleanup();
    enet_deinitialize();
    m_mode = NetworkMode::None;
}

bool NetworkManager::createServer(uint16_t port, int maxClients) {
    if (m_host) {
        cleanup();
    }
    
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;
    
    m_host = enet_host_create(&address, maxClients, 2, 0, 0);
    if (!m_host) {
        RC_ERROR("Failed to create server on port {}", port);
        return false;
    }
    
    m_mode = NetworkMode::Server;
    RC_INFO("Server created on port {} with max {} clients", port, maxClients);
    return true;
}

bool NetworkManager::createClient() {
    if (m_host) {
        cleanup();
    }
    
    m_host = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!m_host) {
        RC_ERROR("Failed to create client");
        return false;
    }
    
    m_mode = NetworkMode::Client;
    RC_INFO("Client created");
    return true;
}

bool NetworkManager::connect(const std::string& host, uint16_t port) {
    if (m_mode != NetworkMode::Client || !m_host) {
        RC_ERROR("Cannot connect: not a client");
        return false;
    }
    
    ENetAddress address;
    enet_address_set_host(&address, host.c_str());
    address.port = port;
    
    m_serverPeer = enet_host_connect(m_host, &address, 2, 0);
    if (!m_serverPeer) {
        RC_ERROR("Failed to initiate connection to {}:{}", host, port);
        return false;
    }
    
    ENetEvent event;
    if (enet_host_service(m_host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        RC_INFO("Connected to {}:{}", host, port);
        return true;
    }
    
    enet_peer_reset(m_serverPeer);
    m_serverPeer = nullptr;
    RC_ERROR("Connection to {}:{} timed out", host, port);
    return false;
}

void NetworkManager::disconnect() {
    if (m_mode == NetworkMode::Client && m_serverPeer) {
        enet_peer_disconnect(m_serverPeer, 0);
        
        ENetEvent event;
        while (enet_host_service(m_host, &event, 3000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    enet_packet_destroy(event.packet);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    RC_INFO("Disconnected from server");
                    m_serverPeer = nullptr;
                    return;
                default:
                    break;
            }
        }
        
        enet_peer_reset(m_serverPeer);
        m_serverPeer = nullptr;
    }
    
    cleanup();
    m_mode = NetworkMode::None;
}

bool NetworkManager::pollEvent(NetworkEvent& event, uint32_t timeoutMs) {
    if (!m_host) return false;
    
    ENetEvent enetEvent;
    int result = enet_host_service(m_host, &enetEvent, timeoutMs);
    
    if (result <= 0) {
        event.type = NetworkEvent::Type::None;
        return false;
    }
    
    switch (enetEvent.type) {
        case ENET_EVENT_TYPE_CONNECT:
            event.type = NetworkEvent::Type::Connect;
            event.peerId = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(enetEvent.peer->data));
            RC_DEBUG("Peer connected: {}", event.peerId);
            if (m_onConnect) m_onConnect(event.peerId);
            break;
            
        case ENET_EVENT_TYPE_DISCONNECT:
            event.type = NetworkEvent::Type::Disconnect;
            event.peerId = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(enetEvent.peer->data));
            RC_DEBUG("Peer disconnected: {}", event.peerId);
            if (m_onDisconnect) m_onDisconnect(event.peerId);
            break;
            
        case ENET_EVENT_TYPE_RECEIVE:
            event.type = NetworkEvent::Type::Receive;
            event.peerId = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(enetEvent.peer->data));
            event.channelId = enetEvent.channelID;
            event.data = enetEvent.packet->data;
            event.dataLength = enetEvent.packet->dataLength;
            if (m_onReceive) m_onReceive(event.peerId, event.data, event.dataLength);
            enet_packet_destroy(enetEvent.packet);
            break;
            
        default:
            event.type = NetworkEvent::Type::None;
            break;
    }
    
    return true;
}

bool NetworkManager::sendToPeer(uint32_t peerId, const void* data, size_t size, uint8_t channel, bool reliable) {
    if (!m_host || m_mode != NetworkMode::Server) return false;
    
    uint32_t flags = reliable ? ENET_PACKET_FLAG_RELIABLE : 0;
    ENetPacket* packet = enet_packet_create(data, size, flags);
    
    for (size_t i = 0; i < m_host->peerCount; ++i) {
        ENetPeer* peer = &m_host->peers[i];
        if (peer->state == ENET_PEER_STATE_CONNECTED) {
            uint32_t id = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(peer->data));
            if (id == peerId) {
                enet_peer_send(peer, channel, packet);
                return true;
            }
        }
    }
    
    enet_packet_destroy(packet);
    return false;
}

bool NetworkManager::broadcast(const void* data, size_t size, uint8_t channel, bool reliable) {
    if (!m_host) return false;
    
    uint32_t flags = reliable ? ENET_PACKET_FLAG_RELIABLE : 0;
    ENetPacket* packet = enet_packet_create(data, size, flags);
    enet_host_broadcast(m_host, channel, packet);
    return true;
}

void NetworkManager::cleanup() {
    if (m_host) {
        enet_host_destroy(m_host);
        m_host = nullptr;
    }
    m_serverPeer = nullptr;
}

}
