#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace roblox_clone::renderer {

class Texture {
public:
    Texture();
    ~Texture();
    
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    
    bool loadFromFile(const std::string& filepath);
    bool create(int width, int height, GLenum format, const void* data);
    
    void bind(GLuint unit = 0) const;
    void unbind() const;
    
    GLuint getHandle() const { return m_handle; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    bool isValid() const { return m_handle != 0; }

private:
    GLuint m_handle = 0;
    int m_width = 0;
    int m_height = 0;
    GLenum m_format = GL_RGBA;
};

using TexturePtr = std::shared_ptr<Texture>;

}
