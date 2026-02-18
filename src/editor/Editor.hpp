#pragma once

#include "renderer/Window.hpp"
#include "renderer/Renderer.hpp"
#include "scene/Scene.hpp"
#include <imgui.h>
#include <memory>

namespace roblox_clone::editor {

class Viewport;

class Editor {
public:
    Editor();
    ~Editor();
    
    bool initialize(renderer::Window* window, renderer::Renderer* renderer);
    void shutdown();
    
    void beginFrame();
    void endFrame();
    
    void render(scene::Scene* scene, float deltaTime);
    void renderViewport();
    
    void showDemoWindow(bool* open = nullptr);
    
    bool isInitialized() const { return m_initialized; }

private:
    void setupStyle();
    void renderMenuBar();
    void renderSceneHierarchy(scene::Scene* scene);
    void renderPropertiesPanel(scene::Scene* scene);
    void renderConsole();
    void renderStats(float deltaTime);
    
    renderer::Window* m_window = nullptr;
    renderer::Renderer* m_renderer = nullptr;
    
    std::unique_ptr<Viewport> m_viewport;
    
    bool m_initialized = false;
    bool m_showDemo = false;
    bool m_showConsole = true;
    bool m_showStats = true;
    bool m_showHierarchy = true;
    bool m_showProperties = true;
    
    scene::Entity m_selectedEntity;
    
    float m_fps = 0.0f;
    float m_frameTime = 0.0f;
    int m_frameCount = 0;
    float m_fpsAccumulator = 0.0f;
};

}
