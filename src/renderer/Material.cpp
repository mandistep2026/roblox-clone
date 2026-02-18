#include "Material.hpp"
#include "Shader.hpp"

namespace roblox_clone::renderer {

Material::Material() {
}

void Material::apply(Shader* shader) const {
    if (!shader) return;
    
    shader->setVec4("material.diffuseColor", m_diffuseColor);
    shader->setVec3("material.specularColor", m_specularColor);
    shader->setFloat("material.shininess", m_shininess);
    
    if (m_diffuseTexture) {
        m_diffuseTexture->bind(0);
        shader->setInt("material.diffuseTexture", 0);
        shader->setInt("material.useDiffuseTexture", 1);
    } else {
        shader->setInt("material.useDiffuseTexture", 0);
    }
    
    if (m_normalTexture) {
        m_normalTexture->bind(1);
        shader->setInt("material.normalTexture", 1);
        shader->setInt("material.useNormalTexture", 1);
    } else {
        shader->setInt("material.useNormalTexture", 0);
    }
}

}
