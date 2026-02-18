#pragma once

#include "Texture.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace roblox_clone::renderer {

class Material {
public:
    Material();
    ~Material() = default;
    
    void setDiffuseColor(const glm::vec4& color) { m_diffuseColor = color; }
    void setSpecularColor(const glm::vec3& color) { m_specularColor = color; }
    void setShininess(float value) { m_shininess = value; }
    void setDiffuseTexture(TexturePtr texture) { m_diffuseTexture = texture; }
    void setNormalTexture(TexturePtr texture) { m_normalTexture = texture; }
    
    const glm::vec4& getDiffuseColor() const { return m_diffuseColor; }
    const glm::vec3& getSpecularColor() const { return m_specularColor; }
    float getShininess() const { return m_shininess; }
    TexturePtr getDiffuseTexture() const { return m_diffuseTexture; }
    TexturePtr getNormalTexture() const { return m_normalTexture; }
    
    bool hasDiffuseTexture() const { return m_diffuseTexture != nullptr; }
    bool hasNormalTexture() const { return m_normalTexture != nullptr; }
    
    void apply(class Shader* shader) const;

private:
    glm::vec4 m_diffuseColor = glm::vec4(1.0f);
    glm::vec3 m_specularColor = glm::vec3(1.0f);
    float m_shininess = 32.0f;
    
    TexturePtr m_diffuseTexture;
    TexturePtr m_normalTexture;
};

using MaterialPtr = std::shared_ptr<Material>;

}
