#include "Mesh.hpp"
#include <cmath>

namespace roblox_clone::renderer {

Mesh::Mesh() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
}

Mesh::~Mesh() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
}

void Mesh::create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    m_indexCount = indices.size();
    
    glBindVertexArray(m_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}

void Mesh::createCube(float size) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    float h = size / 2.0f;
    
    glm::vec3 positions[] = {
        glm::vec3(-h, -h,  h), glm::vec3( h, -h,  h), glm::vec3( h,  h,  h), glm::vec3(-h,  h,  h),
        glm::vec3(-h, -h, -h), glm::vec3(-h,  h, -h), glm::vec3( h,  h, -h), glm::vec3( h, -h, -h),
        glm::vec3(-h,  h, -h), glm::vec3(-h,  h,  h), glm::vec3( h,  h,  h), glm::vec3( h,  h, -h),
        glm::vec3(-h, -h, -h), glm::vec3( h, -h, -h), glm::vec3( h, -h,  h), glm::vec3(-h, -h,  h),
        glm::vec3( h, -h, -h), glm::vec3( h,  h, -h), glm::vec3( h,  h,  h), glm::vec3( h, -h,  h),
        glm::vec3(-h, -h, -h), glm::vec3(-h, -h,  h), glm::vec3(-h,  h,  h), glm::vec3(-h,  h, -h),
    };
    
    glm::vec3 normals[] = {
        glm::vec3(0, 0, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0),
        glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0)
    };
    
    glm::vec2 uvs[] = { glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1) };
    
    for (int face = 0; face < 6; ++face) {
        int baseIndex = face * 4;
        for (int i = 0; i < 4; ++i) {
            Vertex v;
            v.position = positions[baseIndex + i];
            v.normal = normals[face];
            v.texCoords = uvs[i];
            vertices.push_back(v);
        }
        
        uint32_t base = face * 4;
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 0);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
    }
    
    create(vertices, indices);
}

void Mesh::createSphere(float radius, int segments) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    for (int lat = 0; lat <= segments; ++lat) {
        float theta = lat * M_PI / segments;
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);
        
        for (int lon = 0; lon <= segments; ++lon) {
            float phi = lon * 2 * M_PI / segments;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);
            
            Vertex v;
            v.normal = glm::vec3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
            v.position = v.normal * radius;
            v.texCoords = glm::vec2(static_cast<float>(lon) / segments, static_cast<float>(lat) / segments);
            vertices.push_back(v);
        }
    }
    
    for (int lat = 0; lat < segments; ++lat) {
        for (int lon = 0; lon < segments; ++lon) {
            uint32_t first = lat * (segments + 1) + lon;
            uint32_t second = first + segments + 1;
            
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            
            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
        }
    }
    
    create(vertices, indices);
}

void Mesh::createPlane(float width, float height) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    float hw = width / 2.0f;
    float hh = height / 2.0f;
    
    Vertex v;
    v.normal = glm::vec3(0, 1, 0);
    
    v.position = glm::vec3(-hw, 0, -hh);
    v.texCoords = glm::vec2(0, 0);
    vertices.push_back(v);
    
    v.position = glm::vec3(hw, 0, -hh);
    v.texCoords = glm::vec2(1, 0);
    vertices.push_back(v);
    
    v.position = glm::vec3(hw, 0, hh);
    v.texCoords = glm::vec2(1, 1);
    vertices.push_back(v);
    
    v.position = glm::vec3(-hw, 0, hh);
    v.texCoords = glm::vec2(0, 1);
    vertices.push_back(v);
    
    indices = { 0, 1, 2, 0, 2, 3 };
    
    create(vertices, indices);
}

void Mesh::bind() const {
    glBindVertexArray(m_vao);
}

void Mesh::unbind() const {
    glBindVertexArray(0);
}

void Mesh::draw() const {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

}
