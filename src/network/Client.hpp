#pragma once

#include "NetworkManager.hpp"
#include <functional>
#include <thread>
#include <atomic>

namespace roblox_clone::scene { class Scene; }

namespace roblox_clone::network {

struct ClientConfig {
    std::string serverHost = "localhost";
    uint16_t serverPort = 7777;
};

class Client {
public:
    Client();
    ~Client();
    
    bool initialize();
    void shutdown();
    
    bool connect(const std::string& host, uint16_t port);
    void disconnect();
    
    void update();
    
    void setScene(roblox_clone::scene::Scene* scene) { m_scene = scene; }
    
    bool sendInput(const void* data, size_t size);
    bool sendChatMessage(const std::string& message);
    
    bool isConnected() const { return m_network.isConnected(); }
    uint32_t getClientId() const { return m_clientId; }

private:
    void handlePacket(const void* data, size_t size);
    
    NetworkManager m_network;
    roblox_clone::scene::Scene* m_scene = nullptr;
    uint32_t m_clientId = 0;
};

}
