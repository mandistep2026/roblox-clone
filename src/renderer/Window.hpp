#pragma once

#include <SDL.h>
#include <string>
#include <functional>

namespace roblox_clone::renderer {

class Window {
public:
    Window() = default;
    ~Window();
    
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    
    bool initialize(const std::string& title, int width, int height, bool fullscreen);
    void shutdown();
    
    void pollEvents();
    void swapBuffers();
    bool shouldClose() const;
    
    void setCloseCallback(std::function<void()> callback) { m_closeCallback = callback; }
    void setResizeCallback(std::function<void(int, int)> callback) { m_resizeCallback = callback; }
    void setKeyCallback(std::function<void(int, bool)> callback) { m_keyCallback = callback; }
    void setMouseCallback(std::function<void(int, int, int, bool)> callback) { m_mouseCallback = callback; }
    
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    SDL_Window* getHandle() const { return m_window; }
    void* getNativeHandle() const;

private:
    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    int m_width = 0;
    int m_height = 0;
    bool m_shouldClose = false;
    
    std::function<void()> m_closeCallback;
    std::function<void(int, int)> m_resizeCallback;
    std::function<void(int, bool)> m_keyCallback;
    std::function<void(int, int, int, bool)> m_mouseCallback;
};

}
