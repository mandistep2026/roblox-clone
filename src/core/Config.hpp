#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace roblox_clone::core {

class Config {
public:
    bool load(const std::string& filepath);
    bool save(const std::string& filepath);
    
    nlohmann::json& data() { return m_data; }
    const nlohmann::json& data() const { return m_data; }
    
    template<typename T>
    T get(const std::string& key, const T& defaultValue) const {
        if (m_data.contains(key)) {
            return m_data[key].get<T>();
        }
        return defaultValue;
    }
    
    template<typename T>
    void set(const std::string& key, const T& value) {
        m_data[key] = value;
    }

private:
    nlohmann::json m_data;
};

}
