#include "Renderer.hpp"
#include "core/Logger.hpp"
#include "scene/Scene.hpp"
#include "scene/Entity.hpp"
#include <entt/entt.hpp>

#ifdef ROBLOX_CLONE_BUILD_EDITOR
#include "editor/Editor.hpp"
#endif

namespace roblox_clone::renderer {

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::initialize(Window* window) {
    m_window = window;
    m_width = window->getWidth();
    m_height = window->getHeight();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    
    if (!loadDefaultShaders()) {
        RC_ERROR("Failed to load default shaders");
        return false;
    }
    
    m_testCube = std::make_unique<Mesh>();
    m_testCube->createCube(1.0f);
    
    window->setResizeCallback([this](int w, int h) {
        this->resize(w, h);
    });
    
    RC_INFO("Renderer initialized successfully");
    return true;
}

void Renderer::shutdown() {
    m_testCube.reset();
    m_basicShader.reset();
    m_window = nullptr;
}

void Renderer::beginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame() {
}

void Renderer::render(scene::Scene* scene) {
    float aspectRatio = static_cast<float>(m_width) / static_cast<float>(m_height);
    glm::mat4 projection = m_camera.getProjectionMatrix(aspectRatio);
    glm::mat4 view = m_camera.getViewMatrix();
    
    m_basicShader->bind();
    m_basicShader->setMat4("projection", projection);
    m_basicShader->setMat4("view", view);
    
    if (scene) {
        auto& registry = scene->registry();
        auto view = registry.view<scene::TransformComponent>();
        
        for (auto entity : view) {
            auto& transform = view.get<scene::TransformComponent>(entity);
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, transform.position);
            model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
            model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
            model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
            model = glm::scale(model, transform.scale);
            
            m_basicShader->setMat4("model", model);
            m_testCube->draw();
        }
    }
    
    m_basicShader->unbind();
}

#ifdef ROBLOX_CLONE_BUILD_EDITOR
void Renderer::renderEditorOverlay(editor::Editor* editor) {
    if (editor) {
        editor->renderViewport();
    }
}
#endif

void Renderer::resize(int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
}

bool Renderer::loadDefaultShaders() {
    m_basicShader = std::make_unique<Shader>();
    
    if (!m_basicShader->loadFromFiles("assets/shaders/basic.vert", "assets/shaders/basic.frag")) {
        static const char* vertexSource = R"(
            #version 450 core
            layout(location = 0) in vec3 aPosition;
            layout(location = 1) in vec3 aNormal;
            layout(location = 2) in vec2 aTexCoords;
            
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            
            out vec3 FragPos;
            out vec3 Normal;
            
            void main() {
                FragPos = vec3(model * vec4(aPosition, 1.0));
                Normal = mat3(transpose(inverse(model))) * aNormal;
                gl_Position = projection * view * vec4(FragPos, 1.0);
            }
        )";
        
        static const char* fragmentSource = R"(
            #version 450 core
            in vec3 FragPos;
            in vec3 Normal;
            
            out vec4 FragColor;
            
            uniform vec3 lightPos = vec3(5.0, 10.0, 5.0);
            uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
            uniform vec3 objectColor = vec3(0.8, 0.4, 0.2);
            
            void main() {
                vec3 norm = normalize(Normal);
                vec3 lightDir = normalize(lightPos - FragPos);
                
                float ambientStrength = 0.3;
                vec3 ambient = ambientStrength * lightColor;
                
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * lightColor;
                
                vec3 result = (ambient + diffuse) * objectColor;
                FragColor = vec4(result, 1.0);
            }
        )";
        
        return m_basicShader->loadFromSource(vertexSource, fragmentSource);
    }
    
    return true;
}

}
