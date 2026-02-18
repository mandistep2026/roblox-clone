#include <sol/sol.hpp>
#include "scene/Scene.hpp"
#include "scene/Entity.hpp"
#include <glm/glm.hpp>

namespace roblox_clone::scripting {

void registerEntityBindings(sol::state& lua);
void registerMathBindings(sol::state& lua);

void registerAllBindings(sol::state& lua) {
    registerEntityBindings(lua);
    registerMathBindings(lua);
}

void registerEntityBindings(sol::state& lua) {
    auto entityType = lua.new_usertype<roblox_clone::scene::Entity>("Entity");
    
    entityType["getName"] = [](roblox_clone::scene::Entity& e) -> std::string {
        if (e.hasComponent<roblox_clone::scene::NameComponent>()) {
            return e.getComponent<roblox_clone::scene::NameComponent>().name;
        }
        return "";
    };
    
    entityType["getPosition"] = [](roblox_clone::scene::Entity& e) -> glm::vec3 {
        if (e.hasComponent<roblox_clone::scene::TransformComponent>()) {
            return e.getComponent<roblox_clone::scene::TransformComponent>().position;
        }
        return glm::vec3(0.0f);
    };
    
    entityType["setPosition"] = [](roblox_clone::scene::Entity& e, const glm::vec3& pos) {
        if (e.hasComponent<roblox_clone::scene::TransformComponent>()) {
            e.getComponent<roblox_clone::scene::TransformComponent>().position = pos;
        }
    };
    
    entityType["getRotation"] = [](roblox_clone::scene::Entity& e) -> glm::vec3 {
        if (e.hasComponent<roblox_clone::scene::TransformComponent>()) {
            return e.getComponent<roblox_clone::scene::TransformComponent>().rotation;
        }
        return glm::vec3(0.0f);
    };
    
    entityType["setRotation"] = [](roblox_clone::scene::Entity& e, const glm::vec3& rot) {
        if (e.hasComponent<roblox_clone::scene::TransformComponent>()) {
            e.getComponent<roblox_clone::scene::TransformComponent>().rotation = rot;
        }
    };
    
    entityType["getScale"] = [](roblox_clone::scene::Entity& e) -> glm::vec3 {
        if (e.hasComponent<roblox_clone::scene::TransformComponent>()) {
            return e.getComponent<roblox_clone::scene::TransformComponent>().scale;
        }
        return glm::vec3(1.0f);
    };
    
    entityType["setScale"] = [](roblox_clone::scene::Entity& e, const glm::vec3& scale) {
        if (e.hasComponent<roblox_clone::scene::TransformComponent>()) {
            e.getComponent<roblox_clone::scene::TransformComponent>().scale = scale;
        }
    };
}

void registerMathBindings(sol::state& lua) {
    auto vec3Type = lua.new_usertype<glm::vec3>("Vector3Instance");
    
    vec3Type["x"] = &glm::vec3::x;
    vec3Type["y"] = &glm::vec3::y;
    vec3Type["z"] = &glm::vec3::z;
    
    vec3Type[sol::meta_function::addition] = [](const glm::vec3& a, const glm::vec3& b) {
        return a + b;
    };
    
    vec3Type[sol::meta_function::subtraction] = [](const glm::vec3& a, const glm::vec3& b) {
        return a - b;
    };
    
    vec3Type[sol::meta_function::multiplication] = sol::overload(
        [](const glm::vec3& a, float b) { return a * b; },
        [](float a, const glm::vec3& b) { return a * b; },
        [](const glm::vec3& a, const glm::vec3& b) { return a * b; }
    );
    
    vec3Type[sol::meta_function::division] = [](const glm::vec3& a, float b) {
        return a / b;
    };
    
    vec3Type[sol::meta_function::to_string] = [](const glm::vec3& v) {
        return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
    };
}

}
