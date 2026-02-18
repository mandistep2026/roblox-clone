#include "Viewport.hpp"
#include <imgui.h>
#include <SDL.h>

namespace roblox_clone::editor {

Viewport::Viewport() {
}

bool Viewport::initialize(renderer::Renderer* renderer) {
    m_renderer = renderer;
    updateCamera();
    return true;
}

void Viewport::render() {
    ImGui::Begin("Viewport");
    
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    m_isViewportFocused = ImGui::IsWindowFocused();
    
    ImGui::Text("3D Viewport");
    ImGui::Text("Size: %.0f x %.0f", viewportSize.x, viewportSize.y);
    
    if (m_isViewportFocused) {
        handleInput();
    }
    
    ImGui::End();
}

void Viewport::handleInput() {
    ImGuiIO& io = ImGui::GetIO();
    
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        if (!m_isRightMousePressed) {
            m_isRightMousePressed = true;
        }
        
        ImVec2 mouseDelta = io.MouseDelta;
        m_cameraYaw += mouseDelta.x * m_mouseSensitivity;
        m_cameraPitch -= mouseDelta.y * m_mouseSensitivity;
        
        if (m_cameraPitch > 89.0f) m_cameraPitch = 89.0f;
        if (m_cameraPitch < -89.0f) m_cameraPitch = -89.0f;
        
        float speed = m_cameraSpeed * io.DeltaTime;
        if (io.KeysDown[ImGuiKey_W]) {
            m_cameraPosition += m_cameraFront * speed;
        }
        if (io.KeysDown[ImGuiKey_S]) {
            m_cameraPosition -= m_cameraFront * speed;
        }
        if (io.KeysDown[ImGuiKey_A]) {
            m_cameraPosition -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * speed;
        }
        if (io.KeysDown[ImGuiKey_D]) {
            m_cameraPosition += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * speed;
        }
        if (io.KeysDown[ImGuiKey_Q]) {
            m_cameraPosition -= m_cameraUp * speed;
        }
        if (io.KeysDown[ImGuiKey_E]) {
            m_cameraPosition += m_cameraUp * speed;
        }
        
        updateCamera();
    } else {
        m_isRightMousePressed = false;
    }
}

void Viewport::updateCamera() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_cameraYaw)) * cos(glm::radians(m_cameraPitch));
    front.y = sin(glm::radians(m_cameraPitch));
    front.z = sin(glm::radians(m_cameraYaw)) * cos(glm::radians(m_cameraPitch));
    m_cameraFront = glm::normalize(front);
    
    m_cameraTarget = m_cameraPosition + m_cameraFront;
    
    if (m_renderer) {
        renderer::Camera& camera = m_renderer->getCamera();
        camera.position = m_cameraPosition;
        camera.target = m_cameraTarget;
    }
}

void Viewport::setCameraPosition(const glm::vec3& position) {
    m_cameraPosition = position;
    updateCamera();
}

void Viewport::setCameraTarget(const glm::vec3& target) {
    m_cameraTarget = target;
    if (m_renderer) {
        m_renderer->getCamera().target = target;
    }
}

}
