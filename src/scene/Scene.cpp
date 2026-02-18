#include "Scene.hpp"
#include "Entity.hpp"

namespace roblox_clone::scene {

Scene::Scene() {
}

Entity Scene::createEntity(const std::string& name) {
    Entity entity = { m_registry.create(), this };
    entity.addComponent<TransformComponent>();
    entity.addComponent<NameComponent>(name);
    return entity;
}

void Scene::destroyEntity(Entity entity) {
    m_registry.destroy(entity);
}

Entity Scene::getEntityByName(const std::string& name) {
    auto view = m_registry.view<NameComponent>();
    for (auto entity : view) {
        if (view.get<NameComponent>(entity).name == name) {
            return { entity, this };
        }
    }
    return {};
}

void Scene::update(float deltaTime) {
    (void)deltaTime;
}

void Scene::setMainCamera(Entity camera) {
    m_mainCamera = camera;
}

}
