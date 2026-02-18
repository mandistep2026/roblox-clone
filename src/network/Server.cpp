#include "Server.hpp"
#include "core/Logger.hpp"

namespace roblox_clone::network {

Server::Server() = default;

Server::~Server() {
    stop();
    shutdown();
}

bool Server::initialize(const ServerConfig& config) {
    m_config = config;
    
    if (!m_network.initialize()) {
        return false;
    }
    
    if (!m_network.createServer(config.port, config.maxClients)) {
        return false;
    }
    
    m_network.setOnConnect([this](uint32_t peerId) { handleConnect(peerId); });
    m_network.setOnDisconnect([this](uint32_t peerId) { handleDisconnect(peerId); });
    m_network.setOnReceive([this](uint32_t peerId, const void* data, size_t size) {
        handlePacket(peerId, data, size);
    });
    
    RC_INFO("Server initialized on port {}", config.port);
    return true;
}

void Server::shutdown() {
    stop();
    m_network.shutdown();
}

void Server::start() {
    if (m_running) return;
    
    m_running = true;
    m_thread = std::thread(&Server::runThread, this);
    RC_INFO("Server started");
}

void Server::stop() {
    if (!m_running) return;
    
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
    }
    RC_INFO("Server stopped");
}

void Server::tick() {
    NetworkEvent event;
    while (m_network.pollEvent(event, 0)) {
        switch (event.type) {
            case NetworkEvent::Type::Connect:
                handleConnect(event.peerId);
                break;
            case NetworkEvent::Type::Disconnect:
                handleDisconnect(event.peerId);
                break;
            case NetworkEvent::Type::Receive:
                handlePacket(event.peerId, event.data, event.dataLength);
                break;
            default:
                break;
        }
    }
}

void Server::runThread() {
    while (m_running) {
        tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_config.tickRate));
    }
}

void Server::handleConnect(uint32_t peerId) {
    m_clients.push_back(peerId);
    RC_INFO("Client connected: {} (total: {})", peerId, m_clients.size());
    
    if (m_scene) {
    }
}

void Server::handleDisconnect(uint32_t peerId) {
    auto it = std::find(m_clients.begin(), m_clients.end(), peerId);
    if (it != m_clients.end()) {
        m_clients.erase(it);
    }
    RC_INFO("Client disconnected: {} (total: {})", peerId, m_clients.size());
}

void Server::handlePacket(uint32_t peerId, const void* data, size_t size) {
    RC_DEBUG("Received {} bytes from client {}", size, peerId);
}

void Server::broadcastEntitySpawn(uint32_t networkId, const std::string& name) {
    struct {
        uint8_t type = 0x01;
        uint32_t networkId;
        char name[64];
    } packet;
    
    packet.networkId = networkId;
    strncpy(packet.name, name.c_str(), sizeof(packet.name) - 1);
    packet.name[sizeof(packet.name) - 1] = '\0';
    
    m_network.broadcast(&packet, sizeof(packet), 0, true);
}

void Server::broadcastEntityTransform(uint32_t networkId, const float* transform) {
    struct {
        uint8_t type = 0x02;
        uint32_t networkId;
        float transform[10];
    } packet;
    
    packet.networkId = networkId;
    memcpy(packet.transform, transform, sizeof(packet.transform));
    
    m_network.broadcast(&packet, sizeof(packet), 1, false);
}

void Server::broadcastEntityDestroy(uint32_t networkId) {
    struct {
        uint8_t type = 0x03;
        uint32_t networkId;
    } packet;
    
    packet.networkId = networkId;
    m_network.broadcast(&packet, sizeof(packet), 0, true);
}

}
