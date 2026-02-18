#include "Entity.hpp"

namespace roblox_clone::scene {

Entity::Entity(entt::entity handle, Scene* scene)
    : m_handle(handle), m_scene(scene) {
}

}
