#pragma once

#include "renderer/Renderer.hpp"
#include <glm/glm.hpp>

namespace roblox_clone::editor {

class Viewport {
public:
    Viewport();
    ~Viewport() = default;
    
    bool initialize(renderer::Renderer* renderer);
    void render();
    
    void setCameraPosition(const glm::vec3& position);
    void setCameraTarget(const glm::vec3& target);
    
    glm::vec3 getCameraPosition() const { return m_cameraPosition; }
    glm::vec3 getCameraTarget() const { return m_cameraTarget; }

private:
    void handleInput();
    void updateCamera();
    
    renderer::Renderer* m_renderer = nullptr;
    
    glm::vec3 m_cameraPosition = glm::vec3(5.0f, 5.0f, 10.0f);
    glm::vec3 m_cameraTarget = glm::vec3(0.0f);
    glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    float m_cameraYaw = -90.0f;
    float m_cameraPitch = 0.0f;
    float m_cameraSpeed = 5.0f;
    float m_mouseSensitivity = 0.1f;
    
    bool m_isViewportFocused = false;
    bool m_isRightMousePressed = false;
};

}
