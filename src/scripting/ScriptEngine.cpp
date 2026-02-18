#include "ScriptEngine.hpp"
#include "ScriptBindings.hpp"
#include "core/Logger.hpp"

namespace roblox_clone::scripting {

ScriptEngine::ScriptEngine() {
    m_lua = std::make_unique<sol::state>();
}

bool ScriptEngine::initialize() {
    m_lua->open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table, sol::lib::debug);
    
    registerEngineAPI();
    
    RC_INFO("Script engine initialized (Lua {})", LUA_VERSION);
    return true;
}

void ScriptEngine::shutdown() {
    m_lua.reset();
}

bool ScriptEngine::loadScript(const std::string& filepath) {
    try {
        auto result = m_lua->script_file(filepath);
        if (!result.valid()) {
            sol::error err = result;
            RC_ERROR("Script error: {}", err.what());
            return false;
        }
        RC_DEBUG("Loaded script: {}", filepath);
        return true;
    } catch (const sol::error& e) {
        RC_ERROR("Failed to load script {}: {}", filepath, e.what());
        return false;
    }
}

bool ScriptEngine::runScript(const std::string& code) {
    try {
        auto result = m_lua->script(code);
        if (!result.valid()) {
            sol::error err = result;
            RC_ERROR("Script error: {}", err.what());
            return false;
        }
        return true;
    } catch (const sol::error& e) {
        RC_ERROR("Script execution error: {}", e.what());
        return false;
    }
}

void ScriptEngine::registerEngineAPI() {
    (*m_lua)["print"] = [](sol::variadic_args args) {
        std::string output;
        for (auto arg : args) {
            output += arg.get<std::string>();
            output += " ";
        }
        RC_INFO("[Lua] {}", output);
    };
    
    (*m_lua)["Vector3"] = sol::overload(
        []() { return glm::vec3(0.0f); },
        [](float v) { return glm::vec3(v); },
        [](float x, float y, float z) { return glm::vec3(x, y, z); }
    );
    
    auto vec3Type = (*m_lua)["Vector3"].get<sol::usertype<glm::vec3>>();
    vec3Type["x"] = &glm::vec3::x;
    vec3Type["y"] = &glm::vec3::y;
    vec3Type["z"] = &glm::vec3::z;
    vec3Type["length"] = [](const glm::vec3& v) { return glm::length(v); };
    vec3Type["normalize"] = [](const glm::vec3& v) { return glm::normalize(v); };
    vec3Type["dot"] = [](const glm::vec3& a, const glm::vec3& b) { return glm::dot(a, b); };
    vec3Type["cross"] = [](const glm::vec3& a, const glm::vec3& b) { return glm::cross(a, b); };
    
    (*m_lua)["math"]["lerp"] = [](float a, float b, float t) {
        return a + (b - a) * t;
    };
    
    (*m_lua)["game"] = (*m_lua).create_table();
    (*m_lua)["game"]["deltaTime"] = 0.0f;
    (*m_lua)["game"]["time"] = 0.0f;
}

void ScriptEngine::registerSceneAPI(scene::Scene* scene) {
    (*m_lua)["workspace"] = (*m_lua).create_table();
    
    (*m_lua)["workspace"]["findPartByName"] = [scene](const std::string& name) -> sol::object {
        auto entity = scene->getEntityByName(name);
        if (entity) {
            return sol::make_object(*m_lua, entity);
        }
        return sol::nil;
    };
    
    (*m_lua)["workspace"]["createPart"] = [scene](const std::string& name) {
        return scene->createEntity(name);
    };
}

}
