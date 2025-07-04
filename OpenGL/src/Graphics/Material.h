#pragma once
#include "Shader.h"
#include "Texture.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <utility>


class Material
{
public:
    explicit Material(std::shared_ptr<Shader> shader);

    // Set a texture to a uniform name at a given slot
    void SetTexture(const std::string& uniformName, std::shared_ptr<Texture> tex, int slot);

    // Bind shader and all textures with their slots and uniform locations
    void Bind() const;
    // New: Set a custom int uniform
    void SetUniform1i(const std::string& name, int value);

    std::shared_ptr<Shader> GetShader() const { return m_Shader; }
protected:
    void ApplyCustomUniforms() const;

private:
    std::shared_ptr<Shader> m_Shader;

    // Map uniform name to pair of (texture ptr, slot)
    std::unordered_map<std::string, std::pair<std::shared_ptr<Texture>, int>> m_Textures;
    // New: Store custom int uniforms
    std::unordered_map<std::string, int> m_CustomIntUniforms;
};