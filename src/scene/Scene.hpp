#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <string>
#include <functional>

namespace roblox_clone::scene {

struct TransformComponent {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    
    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& pos) : position(pos) {}
};

struct NameComponent {
    std::string name;
    
    NameComponent() = default;
    NameComponent(const NameComponent&) = default;
    NameComponent(const std::string& n) : name(n) {}
};

struct MeshRendererComponent {
    std::string meshPath;
    std::string materialPath;
    bool visible = true;
    bool castShadows = true;
    bool receiveShadows = true;
    
    MeshRendererComponent() = default;
    MeshRendererComponent(const MeshRendererComponent&) = default;
};

struct ScriptComponent {
    std::string scriptPath;
    bool enabled = true;
    
    ScriptComponent() = default;
    ScriptComponent(const std::string& path) : scriptPath(path) {}
};

struct NetworkComponent {
    uint32_t networkId = 0;
    bool isReplicated = true;
    bool isOwned = false;
    
    NetworkComponent() = default;
};

class Entity;

class Scene {
public:
    Scene();
    ~Scene() = default;
    
    Entity createEntity(const std::string& name = "Entity");
    void destroyEntity(Entity entity);
    
    Entity getEntityByName(const std::string& name);
    
    template<typename... Components>
    auto view() {
        return m_registry.view<Components...>();
    }
    
    template<typename... Components>
    auto view() const {
        return m_registry.view<Components...>();
    }
    
    template<typename Func>
    void each(Func&& func) {
        m_registry.each([&](entt::entity entity) {
            if (m_registry.all_of<TransformComponent>(entity)) {
                func(entity, m_registry.get<TransformComponent>(entity));
            }
        });
    }
    
    entt::registry& registry() { return m_registry; }
    const entt::registry& registry() const { return m_registry; }
    
    void update(float deltaTime);
    void setMainCamera(Entity camera);
    Entity getMainCamera() const { return m_mainCamera; }

private:
    entt::registry m_registry;
    Entity m_mainCamera;
    
    friend class Entity;
};

class Entity {
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);
    
    template<typename T, typename... Args>
    T& addComponent(Args&&... args) {
        return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
    }
    
    template<typename T>
    T& getComponent() {
        return m_scene->m_registry.get<T>(m_handle);
    }
    
    template<typename T>
    bool hasComponent() const {
        return m_scene->m_registry.all_of<T>(m_handle);
    }
    
    template<typename T>
    void removeComponent() {
        m_scene->m_registry.remove<T>(m_handle);
    }
    
    operator bool() const { return m_handle != entt::null && m_scene; }
    operator entt::entity() const { return m_handle; }
    
    bool operator==(const Entity& other) const {
        return m_handle == other.m_handle && m_scene == other.m_scene;
    }
    
    Scene* getScene() const { return m_scene; }

private:
    entt::entity m_handle = entt::null;
    Scene* m_scene = nullptr;
};

}
