#pragma once

#include <sol/sol.hpp>
#include <string>
#include <memory>

namespace roblox_clone::scene { class Scene; }

namespace roblox_clone::scripting {

class ScriptEngine {
public:
    ScriptEngine();
    ~ScriptEngine() = default;
    
    bool initialize();
    void shutdown();
    
    bool loadScript(const std::string& filepath);
    bool runScript(const std::string& code);
    
    sol::state& getState() { return *m_lua; }
    
    void registerEngineAPI();
    void registerSceneAPI(scene::Scene* scene);
    
    template<typename T>
    void setGlobal(const std::string& name, T&& value) {
        (*m_lua)[name] = std::forward<T>(value);
    }

private:
    std::unique_ptr<sol::state> m_lua;
};

}
