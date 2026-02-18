#include "Config.hpp"
#include "Logger.hpp"
#include <fstream>

namespace roblox_clone::core {

bool Config::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        RC_WARN("Failed to open config file: {}", filepath);
        return false;
    }
    
    try {
        file >> m_data;
        RC_INFO("Loaded config from: {}", filepath);
        return true;
    } catch (const nlohmann::json::parse_error& e) {
        RC_ERROR("Failed to parse config file: {}", e.what());
        return false;
    }
}

bool Config::save(const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        RC_ERROR("Failed to create config file: {}", filepath);
        return false;
    }
    
    file << m_data.dump(4);
    RC_INFO("Saved config to: {}", filepath);
    return true;
}

}
