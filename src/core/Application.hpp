#pragma once

#include "Config.hpp"
#include "renderer/Window.hpp"
#include "renderer/Renderer.hpp"
#include "scene/Scene.hpp"
#include "scripting/ScriptEngine.hpp"
#include "network/NetworkManager.hpp"

#ifdef ROBLOX_CLONE_BUILD_EDITOR
#include "editor/Editor.hpp"
#endif

#include <memory>
#include <string>

namespace roblox_clone::core {

struct AppConfig {
    std::string title = "Roblox Clone";
    int width = 1280;
    int height = 720;
    bool fullscreen = false;
    bool vsync = true;
    bool editorMode = true;
};

class Application {
public:
    Application() = default;
    ~Application();
    
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    
    bool initialize(int argc, char* argv[]);
    int run();
    void close();
    
    static Application* getInstance() { return s_instance; }
    
    renderer::Window* getWindow() const { return m_window.get(); }
    renderer::Renderer* getRenderer() const { return m_renderer.get(); }
    scene::Scene* getScene() const { return m_scene.get(); }
    scripting::ScriptEngine* getScriptEngine() const { return m_scriptEngine.get(); }
    network::NetworkManager* getNetworkManager() const { return m_networkManager.get(); }

private:
    bool loadConfig(const std::string& filepath);
    void processCommandLine(int argc, char* argv[]);
    void mainLoop();
    
    static Application* s_instance;
    
    AppConfig m_config;
    std::unique_ptr<renderer::Window> m_window;
    std::unique_ptr<renderer::Renderer> m_renderer;
    std::unique_ptr<scene::Scene> m_scene;
    std::unique_ptr<scripting::ScriptEngine> m_scriptEngine;
    std::unique_ptr<network::NetworkManager> m_networkManager;
    
#ifdef ROBLOX_CLONE_BUILD_EDITOR
    std::unique_ptr<editor::Editor> m_editor;
#endif
    
    bool m_running = false;
};

}
