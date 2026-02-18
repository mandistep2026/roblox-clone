#pragma once

#include "scene/Scene.hpp"
#include <string>

namespace roblox_clone::scene {

Entity createEntity(Scene* scene, const std::string& name);
void destroyEntity(Scene* scene, Entity entity);
Entity findEntityByName(Scene* scene, const std::string& name);

}
