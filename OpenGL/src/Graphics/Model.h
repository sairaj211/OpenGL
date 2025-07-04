#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include <assimp/scene.h>

class Model
{
public:
    Model(const std::string& path);

    void Draw(const Renderer& renderer,
        const glm::mat4& modelMatrix,
        const glm::mat4& viewProj,
        const glm::vec3& lightPos,
        const glm::vec4& lightColor,
        const glm::vec3& camPos,
        std::shared_ptr<Material> overrideMaterial = nullptr);

    void SetMaterial(std::shared_ptr<Material> material);

private:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:
    std::vector<std::unique_ptr<Mesh>> m_Meshes;

    std::shared_ptr<Material> m_CustomMaterial = nullptr;
    std::shared_ptr<Material> m_DefaultMaterial = nullptr;
};