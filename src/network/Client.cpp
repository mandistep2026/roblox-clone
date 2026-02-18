#include "Client.hpp"
#include "core/Logger.hpp"

namespace roblox_clone::network {

Client::Client() = default;

Client::~Client() {
    disconnect();
    shutdown();
}

bool Client::initialize() {
    if (!m_network.initialize()) {
        return false;
    }
    
    if (!m_network.createClient()) {
        return false;
    }
    
    m_network.setOnReceive([this](uint32_t, const void* data, size_t size) {
        handlePacket(data, size);
    });
    
    RC_INFO("Client initialized");
    return true;
}

void Client::shutdown() {
    disconnect();
    m_network.shutdown();
}

bool Client::connect(const std::string& host, uint16_t port) {
    if (!m_network.connect(host, port)) {
        RC_ERROR("Failed to connect to {}:{}", host, port);
        return false;
    }
    
    RC_INFO("Connected to {}:{}", host, port);
    return true;
}

void Client::disconnect() {
    m_network.disconnect();
    RC_INFO("Disconnected from server");
}

void Client::update() {
    NetworkEvent event;
    while (m_network.pollEvent(event, 0)) {
        if (event.type == NetworkEvent::Type::Receive) {
            handlePacket(event.data, event.dataLength);
        } else if (event.type == NetworkEvent::Type::Disconnect) {
            RC_WARN("Disconnected from server");
        }
    }
}

void Client::handlePacket(const void* data, size_t size) {
    if (size < 1) return;
    
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    uint8_t type = bytes[0];
    
    switch (type) {
        case 0x01: {
            if (size >= sizeof(uint8_t) + sizeof(uint32_t) + 64) {
                struct {
                    uint8_t type;
                    uint32_t networkId;
                    char name[64];
                } const* spawnPacket = static_cast<decltype(spawnPacket)>(data);
                
                RC_DEBUG("Entity spawned: {} (id: {})", spawnPacket->name, spawnPacket->networkId);
            }
            break;
        }
        case 0x02: {
            if (size >= sizeof(uint8_t) + sizeof(uint32_t) + sizeof(float) * 10) {
                RC_DEBUG("Entity transform update received");
            }
            break;
        }
        case 0x03: {
            if (size >= sizeof(uint8_t) + sizeof(uint32_t)) {
                struct {
                    uint8_t type;
                    uint32_t networkId;
                } const* destroyPacket = static_cast<decltype(destroyPacket)>(data);
                
                RC_DEBUG("Entity destroyed: {}", destroyPacket->networkId);
            }
            break;
        }
        default:
            RC_WARN("Unknown packet type: {}", type);
            break;
    }
}

bool Client::sendInput(const void* data, size_t size) {
    if (!m_network.isConnected()) return false;
    
    struct {
        uint8_t type = 0x10;
    } header;
    
    std::vector<uint8_t> packet(sizeof(header) + size);
    memcpy(packet.data(), &header, sizeof(header));
    memcpy(packet.data() + sizeof(header), data, size);
    
    return m_network.sendToPeer(0, packet.data(), packet.size(), 1, false);
}

bool Client::sendChatMessage(const std::string& message) {
    if (!m_network.isConnected()) return false;
    
    struct {
        uint8_t type = 0x11;
        char message[256];
    } packet;
    
    strncpy(packet.message, message.c_str(), sizeof(packet.message) - 1);
    packet.message[sizeof(packet.message) - 1] = '\0';
    
    return m_network.sendToPeer(0, &packet, sizeof(packet), 0, true);
}

}
