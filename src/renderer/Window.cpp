#include "Window.hpp"
#include "core/Logger.hpp"
#include <GL/glew.h>

namespace roblox_clone::renderer {

Window::~Window() {
    shutdown();
}

bool Window::initialize(const std::string& title, int width, int height, bool fullscreen) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
        RC_ERROR("Failed to initialize SDL: {}", SDL_GetError());
        return false;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    
    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    
    m_window = SDL_CreateWindow(title.c_str(), 
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 width, height, flags);
    if (!m_window) {
        RC_ERROR("Failed to create window: {}", SDL_GetError());
        SDL_Quit();
        return false;
    }
    
    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext) {
        RC_ERROR("Failed to create OpenGL context: {}", SDL_GetError());
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    
    SDL_GL_MakeCurrent(m_window, m_glContext);
    SDL_GL_SetSwapInterval(1);
    
    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();
    if (glewErr != GLEW_OK) {
        RC_ERROR("Failed to initialize GLEW: {}", glewGetErrorString(glewErr));
        SDL_GL_DeleteContext(m_glContext);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    
    glGetError();
    
    m_width = width;
    m_height = height;
    
    RC_INFO("Window created: {}x{} | OpenGL {}", width, height, glGetString(GL_VERSION));
    RC_INFO("Renderer: {} | Vendor: {}", glGetString(GL_RENDERER), glGetString(GL_VENDOR));
    
    return true;
}

void Window::shutdown() {
    if (m_glContext) {
        SDL_GL_DeleteContext(m_glContext);
        m_glContext = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
}

void Window::pollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                m_shouldClose = true;
                if (m_closeCallback) m_closeCallback();
                break;
                
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    m_width = event.window.data1;
                    m_height = event.window.data2;
                    glViewport(0, 0, m_width, m_height);
                    if (m_resizeCallback) m_resizeCallback(m_width, m_height);
                }
                break;
                
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if (m_keyCallback) {
                    m_keyCallback(static_cast<int>(event.key.keysym.scancode), 
                                  event.type == SDL_KEYDOWN);
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                if (m_mouseCallback) {
                    m_mouseCallback(static_cast<int>(event.button.button),
                                    event.button.x, event.button.y,
                                    event.type == SDL_MOUSEBUTTONDOWN);
                }
                break;
                
            case SDL_MOUSEMOTION:
                break;
                
            default:
                break;
        }
    }
}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(m_window);
}

bool Window::shouldClose() const {
    return m_shouldClose;
}

void* Window::getNativeHandle() const {
    return static_cast<void*>(m_window);
}

}
