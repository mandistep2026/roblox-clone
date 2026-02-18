#include "Texture.hpp"
#include "core/Logger.hpp"
#include <stb_image.h>

namespace roblox_clone::renderer {

Texture::Texture() {
    glGenTextures(1, &m_handle);
}

Texture::~Texture() {
    if (m_handle) {
        glDeleteTextures(1, &m_handle);
    }
}

bool Texture::loadFromFile(const std::string& filepath) {
    stbi_set_flip_vertically_on_load(true);
    
    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 4);
    
    if (!data) {
        RC_ERROR("Failed to load texture: {}", filepath);
        return false;
    }
    
    GLenum format = GL_RGBA;
    if (channels == 3) format = GL_RGB;
    else if (channels == 1) format = GL_RED;
    
    bool result = create(width, height, format, data);
    
    stbi_image_free(data);
    
    if (result) {
        RC_DEBUG("Loaded texture: {} ({}x{})", filepath, width, height);
    }
    
    return result;
}

bool Texture::create(int width, int height, GLenum format, const void* data) {
    m_width = width;
    m_height = height;
    m_format = format;
    
    glBindTexture(GL_TEXTURE_2D, m_handle);
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return true;
}

void Texture::bind(GLuint unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

}
