#include "Shader.hpp"
#include "core/Logger.hpp"
#include <fstream>
#include <sstream>

namespace roblox_clone::renderer {

Shader::~Shader() {
    if (m_program) {
        glDeleteProgram(m_program);
    }
}

bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);
    
    if (!vertexFile.is_open()) {
        RC_ERROR("Failed to open vertex shader: {}", vertexPath);
        return false;
    }
    if (!fragmentFile.is_open()) {
        RC_ERROR("Failed to open fragment shader: {}", fragmentPath);
        return false;
    }
    
    std::stringstream vertexStream, fragmentStream;
    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();
    
    return loadFromSource(vertexStream.str(), fragmentStream.str());
}

bool Shader::loadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) return false;
    
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader) {
        glDeleteShader(vertexShader);
        return false;
    }
    
    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);
    
    GLint success;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_program, 512, nullptr, infoLog);
        RC_ERROR("Shader program linking failed: {}", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(m_program);
        m_program = 0;
        return false;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    RC_DEBUG("Shader program created successfully");
    return true;
}

void Shader::bind() const {
    glUseProgram(m_program);
}

void Shader::unbind() const {
    glUseProgram(0);
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) {
    glUniform2fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

GLuint Shader::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        RC_ERROR("Shader compilation failed: {}", infoLog);
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

GLint Shader::getUniformLocation(const std::string& name) {
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end()) {
        return it->second;
    }
    
    GLint location = glGetUniformLocation(m_program, name.c_str());
    m_uniformCache[name] = location;
    return location;
}

}
