#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace roblox_clone::renderer {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
public:
    Mesh();
    ~Mesh();
    
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    
    void create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    void createCube(float size = 1.0f);
    void createSphere(float radius = 1.0f, int segments = 32);
    void createPlane(float width = 10.0f, float height = 10.0f);
    
    void bind() const;
    void unbind() const;
    void draw() const;
    
    GLuint getVAO() const { return m_vao; }
    size_t getIndexCount() const { return m_indexCount; }
    bool isValid() const { return m_vao != 0; }

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
    size_t m_indexCount = 0;
};

}
