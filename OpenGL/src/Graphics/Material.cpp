#include "Material.h"

Material::Material(std::shared_ptr<Shader> shader)
    : m_Shader(std::move(shader))
{
}

void Material::SetTexture(const std::string& uniformName, std::shared_ptr<Texture> tex, int slot)
{
    m_Textures[uniformName] = { tex, slot };
}

void Material::Bind() const
{
    m_Shader->Bind();
    for (auto& kv : m_Textures)
    {
        const std::string& name = kv.first;
        auto& pair = kv.second;

        pair.first->Bind(pair.second);
        m_Shader->SetUniform1i(name, pair.second);
    }
    ApplyCustomUniforms();
}

void Material::SetUniform1i(const std::string& name, int value)
{
    m_CustomIntUniforms[name] = value;
}

void Material::ApplyCustomUniforms() const
{
    for (auto& kv : m_CustomIntUniforms)
    {
        const std::string& name = kv.first;
        int value = kv.second;
        m_Shader->SetUniform1i(name, value);
    }
}