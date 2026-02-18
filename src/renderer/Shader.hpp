#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace roblox_clone::renderer {

class Shader {
public:
    Shader() = default;
    ~Shader();
    
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    
    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    bool loadFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    
    void bind() const;
    void unbind() const;
    
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat4(const std::string& name, const glm::mat4& value);
    
    GLuint getProgram() const { return m_program; }
    bool isValid() const { return m_program != 0; }

private:
    GLuint compileShader(GLenum type, const std::string& source);
    GLint getUniformLocation(const std::string& name);
    
    GLuint m_program = 0;
    std::unordered_map<std::string, GLint> m_uniformCache;
};

}
