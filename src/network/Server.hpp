#pragma once

#include "NetworkManager.hpp"
#include <functional>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>

namespace roblox_clone::scene { class Scene; }

namespace roblox_clone::network {

struct ServerConfig {
    uint16_t port = 7777;
    int maxClients = 32;
    int tickRate = 60;
};

class Server {
public:
    Server();
    ~Server();
    
    bool initialize(const ServerConfig& config);
    void shutdown();
    
    void start();
    void stop();
    void tick();
    
    void setScene(roblox_clone::scene::Scene* scene) { m_scene = scene; }
    
    void broadcastEntitySpawn(uint32_t networkId, const std::string& name);
    void broadcastEntityTransform(uint32_t networkId, const float* transform);
    void broadcastEntityDestroy(uint32_t networkId);
    
    size_t getClientCount() const { return m_clients.size(); }
    bool isRunning() const { return m_running; }

private:
    void runThread();
    void handleConnect(uint32_t peerId);
    void handleDisconnect(uint32_t peerId);
    void handlePacket(uint32_t peerId, const void* data, size_t size);
    
    NetworkManager m_network;
    ServerConfig m_config;
    roblox_clone::scene::Scene* m_scene = nullptr;
    
    std::thread m_thread;
    std::atomic<bool> m_running{false};
    
    std::vector<uint32_t> m_clients;
    uint32_t m_nextClientId = 1;
};

}
