#include "Application.hpp"
#include "Logger.hpp"

#include <chrono>

namespace roblox_clone::core {

Application* Application::s_instance = nullptr;

Application::~Application() {
    close();
}

bool Application::initialize(int argc, char* argv[]) {
    s_instance = this;
    
    processCommandLine(argc, argv);
    loadConfig("config.json");
    
    RC_INFO("Initializing Roblox Clone Engine v0.1.0");
    
    m_window = std::make_unique<renderer::Window>();
    if (!m_window->initialize(m_config.title, m_config.width, m_config.height, m_config.fullscreen)) {
        RC_ERROR("Failed to initialize window");
        return false;
    }
    
    m_renderer = std::make_unique<renderer::Renderer>();
    if (!m_renderer->initialize(m_window.get())) {
        RC_ERROR("Failed to initialize renderer");
        return false;
    }
    
    m_scene = std::make_unique<scene::Scene>();
    
    m_scriptEngine = std::make_unique<scripting::ScriptEngine>();
    if (!m_scriptEngine->initialize()) {
        RC_ERROR("Failed to initialize script engine");
        return false;
    }
    
    m_networkManager = std::make_unique<network::NetworkManager>();
    
#ifdef ROBLOX_CLONE_BUILD_EDITOR
    if (m_config.editorMode) {
        m_editor = std::make_unique<editor::Editor>();
        if (!m_editor->initialize(m_window.get(), m_renderer.get())) {
            RC_ERROR("Failed to initialize editor");
            return false;
        }
    }
#endif
    
    m_running = true;
    RC_INFO("Engine initialized successfully");
    return true;
}

int Application::run() {
    auto lastTime = std::chrono::high_resolution_clock::now();
    
    while (m_running && !m_window->shouldClose()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        m_window->pollEvents();
        
        m_scene->update(deltaTime);
        
#ifdef ROBLOX_CLONE_BUILD_EDITOR
        if (m_editor) {
            m_editor->beginFrame();
            m_editor->render(m_scene.get(), deltaTime);
            m_editor->endFrame();
        }
#endif
        
        m_renderer->beginFrame();
        m_renderer->render(m_scene.get());
        
#ifdef ROBLOX_CLONE_BUILD_EDITOR
        if (m_editor) {
            m_renderer->renderEditorOverlay(m_editor.get());
        }
#endif
        
        m_renderer->endFrame();
        m_window->swapBuffers();
    }
    
    return 0;
}

void Application::close() {
    if (!m_running) return;
    
    RC_INFO("Shutting down engine...");
    m_running = false;
    
#ifdef ROBLOX_CLONE_BUILD_EDITOR
    m_editor.reset();
#endif
    
    m_networkManager.reset();
    m_scriptEngine.reset();
    m_scene.reset();
    m_renderer.reset();
    m_window.reset();
    
    s_instance = nullptr;
    RC_INFO("Engine shutdown complete");
}

bool Application::loadConfig(const std::string& filepath) {
    Config config;
    if (config.load(filepath)) {
        m_config.title = config.get<std::string>("title", m_config.title);
        m_config.width = config.get<int>("width", m_config.width);
        m_config.height = config.get<int>("height", m_config.height);
        m_config.fullscreen = config.get<bool>("fullscreen", m_config.fullscreen);
        m_config.vsync = config.get<bool>("vsync", m_config.vsync);
        m_config.editorMode = config.get<bool>("editorMode", m_config.editorMode);
    }
    return true;
}

void Application::processCommandLine(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--no-editor") {
            m_config.editorMode = false;
        } else if (arg == "--fullscreen") {
            m_config.fullscreen = true;
        }
    }
}

}
