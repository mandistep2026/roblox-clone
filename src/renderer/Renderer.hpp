#pragma once

#include "Window.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace roblox_clone::scene { class Scene; }

#ifdef ROBLOX_CLONE_BUILD_EDITOR
namespace roblox_clone::editor { class Editor; }
#endif

namespace roblox_clone::renderer {

struct Camera {
    glm::vec3 position = glm::vec3(0, 5, 10);
    glm::vec3 target = glm::vec3(0, 0, 0);
    glm::vec3 up = glm::vec3(0, 1, 0);
    float fov = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
};

class Renderer {
public:
    Renderer() = default;
    ~Renderer();
    
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    
    bool initialize(Window* window);
    void shutdown();
    
    void beginFrame();
    void endFrame();
    
    void render(scene::Scene* scene);
    
#ifdef ROBLOX_CLONE_BUILD_EDITOR
    void renderEditorOverlay(editor::Editor* editor);
#endif
    
    void setCamera(const Camera& camera) { m_camera = camera; }
    Camera& getCamera() { return m_camera; }
    const Camera& getCamera() const { return m_camera; }
    
    void resize(int width, int height);

private:
    bool loadDefaultShaders();
    void renderMesh(Mesh* mesh, const glm::mat4& transform);
    
    Window* m_window = nullptr;
    Camera m_camera;
    
    std::unique_ptr<Shader> m_basicShader;
    std::unique_ptr<Mesh> m_testCube;
    
    int m_width = 1280;
    int m_height = 720;
};

}
