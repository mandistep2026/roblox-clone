#include "Editor.hpp"
#include "Viewport.hpp"
#include "core/Logger.hpp"
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

namespace roblox_clone::editor {

Editor::Editor() {
    m_viewport = std::make_unique<Viewport>();
}

Editor::~Editor() {
    shutdown();
}

bool Editor::initialize(renderer::Window* window, renderer::Renderer* renderer) {
    m_window = window;
    m_renderer = renderer;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    setupStyle();
    
    if (!ImGui_ImplSDL3_InitForOpenGL(window->getHandle(), window->getNativeHandle())) {
        RC_ERROR("Failed to initialize ImGui SDL3 backend");
        return false;
    }
    
    if (!ImGui_ImplOpenGL3_Init("#version 450")) {
        RC_ERROR("Failed to initialize ImGui OpenGL3 backend");
        return false;
    }
    
    if (!m_viewport->initialize(renderer)) {
        RC_ERROR("Failed to initialize viewport");
        return false;
    }
    
    m_initialized = true;
    RC_INFO("Editor initialized");
    return true;
}

void Editor::shutdown() {
    if (!m_initialized) return;
    
    m_viewport.reset();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    
    m_initialized = false;
}

void Editor::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void Editor::endFrame() {
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        SDL_Window* backupWindow = SDL_GL_GetCurrentWindow();
        SDL_GLContext backupContext = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backupWindow, backupContext);
    }
}

void Editor::render(scene::Scene* scene, float deltaTime) {
    m_fpsAccumulator += deltaTime;
    m_frameCount++;
    
    if (m_fpsAccumulator >= 1.0f) {
        m_fps = static_cast<float>(m_frameCount) / m_fpsAccumulator;
        m_frameTime = m_fpsAccumulator / static_cast<float>(m_frameCount) * 1000.0f;
        m_frameCount = 0;
        m_fpsAccumulator = 0.0f;
    }
    
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    
    renderMenuBar();
    
    if (m_showHierarchy) {
        renderSceneHierarchy(scene);
    }
    
    if (m_showProperties) {
        renderPropertiesPanel(scene);
    }
    
    if (m_showConsole) {
        renderConsole();
    }
    
    if (m_showStats) {
        renderStats(deltaTime);
    }
    
    if (m_showDemo) {
        ImGui::ShowDemoWindow(&m_showDemo);
    }
}

void Editor::renderViewport() {
    if (m_viewport) {
        m_viewport->render();
    }
}

void Editor::setupStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 3.0f;
    
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.24f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.24f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.26f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.32f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
}

void Editor::renderMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {}
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Scene Hierarchy", nullptr, &m_showHierarchy);
            ImGui::MenuItem("Properties", nullptr, &m_showProperties);
            ImGui::MenuItem("Console", nullptr, &m_showConsole);
            ImGui::MenuItem("Stats", nullptr, &m_showStats);
            ImGui::Separator();
            ImGui::MenuItem("ImGui Demo", nullptr, &m_showDemo);
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Play")) {
            if (ImGui::MenuItem("Play", "F5")) {}
            if (ImGui::MenuItem("Pause", "F6")) {}
            if (ImGui::MenuItem("Stop", "F7")) {}
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}

void Editor::renderSceneHierarchy(scene::Scene* scene) {
    ImGui::Begin("Scene Hierarchy", &m_showHierarchy);
    
    if (ImGui::Button("Create Entity")) {
        if (scene) {
            auto entity = scene->createEntity("New Entity");
            m_selectedEntity = entity;
        }
    }
    
    ImGui::Separator();
    
    if (scene) {
        auto view = scene->view<scene::NameComponent, scene::TransformComponent>();
        for (auto entity : view) {
            auto& name = scene->registry().get<scene::NameComponent>(entity);
            
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
            if (m_selectedEntity == scene::Entity{entity, scene}) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            
            if (ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<intptr_t>(entity)), flags, "%s", name.name.c_str())) {
                ImGui::TreePop();
            }
            
            if (ImGui::IsItemClicked()) {
                m_selectedEntity = scene::Entity{entity, scene};
            }
        }
    }
    
    ImGui::End();
}

void Editor::renderPropertiesPanel(scene::Scene* scene) {
    ImGui::Begin("Properties", &m_showProperties);
    
    if (m_selectedEntity && m_selectedEntity.getScene() == scene) {
        if (m_selectedEntity.hasComponent<scene::NameComponent>()) {
            auto& name = m_selectedEntity.getComponent<scene::NameComponent>();
            char buffer[256];
            strncpy(buffer, name.name.c_str(), sizeof(buffer));
            if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
                name.name = buffer;
            }
        }
        
        if (m_selectedEntity.hasComponent<scene::TransformComponent>()) {
            auto& transform = m_selectedEntity.getComponent<scene::TransformComponent>();
            
            ImGui::Separator();
            ImGui::Text("Transform");
            
            ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
            ImGui::DragFloat3("Rotation", &transform.rotation.x, 1.0f);
            ImGui::DragFloat3("Scale", &transform.scale.x, 0.1f);
        }
    } else {
        ImGui::Text("No entity selected");
    }
    
    ImGui::End();
}

void Editor::renderConsole() {
    ImGui::Begin("Console", &m_showConsole);
    
    static char input[256] = "";
    if (ImGui::InputText("Command", input, sizeof(input), ImGuiInputTextFlags_EnterReturnsTrue)) {
        RC_INFO("> {}", input);
        input[0] = '\0';
    }
    
    ImGui::Separator();
    ImGui::BeginChild("ConsoleOutput");
    ImGui::Text("Console output will appear here...");
    ImGui::EndChild();
    
    ImGui::End();
}

void Editor::renderStats(float deltaTime) {
    ImGui::Begin("Stats", &m_showStats);
    
    ImGui::Text("FPS: %.1f", m_fps);
    ImGui::Text("Frame Time: %.2f ms", m_frameTime);
    ImGui::Text("Delta Time: %.4f s", deltaTime);
    
    if (m_renderer) {
        auto& camera = m_renderer->getCamera();
        ImGui::Separator();
        ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)", 
                    camera.position.x, camera.position.y, camera.position.z);
    }
    
    ImGui::End();
}

void Editor::showDemoWindow(bool* open) {
    m_showDemo = true;
    if (open) *open = true;
}

}
