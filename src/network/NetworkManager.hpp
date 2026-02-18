#pragma once

#include <enet/enet.h>
#include <string>
#include <functional>
#include <memory>

namespace roblox_clone::network {

enum class NetworkMode {
    None,
    Server,
    Client
};

struct NetworkConfig {
    uint16_t port = 7777;
    int maxClients = 32;
    int channels = 2;
    int incomingBandwidth = 0;
    int outgoingBandwidth = 0;
};

struct NetworkEvent {
    enum class Type {
        None,
        Connect,
        Disconnect,
        Receive
    };
    
    Type type = Type::None;
    uint32_t peerId = 0;
    uint8_t channelId = 0;
    const void* data = nullptr;
    size_t dataLength = 0;
};

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();
    
    bool initialize();
    void shutdown();
    
    bool createServer(uint16_t port, int maxClients = 32);
    bool createClient();
    bool connect(const std::string& host, uint16_t port);
    void disconnect();
    
    bool pollEvent(NetworkEvent& event, uint32_t timeoutMs = 0);
    
    bool sendToPeer(uint32_t peerId, const void* data, size_t size, uint8_t channel = 0, bool reliable = true);
    bool broadcast(const void* data, size_t size, uint8_t channel = 0, bool reliable = true);
    
    void setOnConnect(std::function<void(uint32_t)> callback) { m_onConnect = callback; }
    void setOnDisconnect(std::function<void(uint32_t)> callback) { m_onDisconnect = callback; }
    void setOnReceive(std::function<void(uint32_t, const void*, size_t)> callback) { m_onReceive = callback; }
    
    NetworkMode getMode() const { return m_mode; }
    bool isServer() const { return m_mode == NetworkMode::Server; }
    bool isClient() const { return m_mode == NetworkMode::Client; }
    bool isConnected() const { return m_mode != NetworkMode::None; }

private:
    void cleanup();
    
    ENetHost* m_host = nullptr;
    ENetPeer* m_serverPeer = nullptr;
    NetworkMode m_mode = NetworkMode::None;
    
    std::function<void(uint32_t)> m_onConnect;
    std::function<void(uint32_t)> m_onDisconnect;
    std::function<void(uint32_t, const void*, size_t)> m_onReceive;
};

}
