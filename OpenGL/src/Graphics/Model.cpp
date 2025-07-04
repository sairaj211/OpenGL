#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Texture.h"
#include <iostream>
#include "Material.h"

Model::Model(const std::string& path)
{
    // Create default material
    auto defaultShader = std::make_shared<Shader>("res/shaders/default.shader");
    m_DefaultMaterial = std::make_shared<Material>(defaultShader);

    LoadModel(path);
}

void Model::LoadModel(const std::string& path)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace |
        aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // Process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }

    // Then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

std::unique_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // Positions
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        // Normals
        if (mesh->HasNormals())
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        else
            vertex.normal = glm::vec3(0.0f);

        // Colors (optional)
        if (mesh->HasVertexColors(0))
            vertex.color = glm::vec3(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b);
        else
            vertex.color = glm::vec3(1.0f); // default white

        // Texture Coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coords?
            vertex.texUV = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            vertex.texUV = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Setup vertex buffer layout matching your Vertex struct
    VertexBufferLayout layout;
    layout.Push<float>(3); // position
    layout.Push<float>(3); // normal
    layout.Push<float>(3); // color
    layout.Push<float>(2); // texUV

    // Create the mesh object
    return std::make_unique<Mesh>(vertices.data(), vertices.size() * sizeof(Vertex), indices.data(), (unsigned int)indices.size(), layout);
}

//void Model::Draw(const Renderer& renderer, const glm::mat4& modelMatrix, const glm::mat4& viewProj,
//    const glm::vec3& lightPos, const glm::vec4& lightColor, const glm::vec3& camPos)
//{
//    static std::shared_ptr<Shader> defaultShader = std::make_shared<Shader>("res/shaders/default.shader");
//    static std::shared_ptr<Material> defaultMaterial = std::make_shared<Material>(defaultShader);
//
//    //defaultShader->Bind();
//    //defaultShader->SetUniformMat4("u_Model", modelMatrix);
//    //defaultShader->SetUniformMat4("u_ViewProj", viewProj);
//    //defaultShader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f); // White
//
//    for (auto& mesh : m_Meshes)
//    {
//        // You may want to have materials and bind them here if loaded
//        // For now just use a default shader/material
//
//        // If you want to integrate materials:
//        // m_Materials[i]->Bind();
//        // auto shader = m_Materials[i]->GetShader();
//        // shader->SetUniformMat4("model", modelMatrix);
//        // ...
//
//        // For simplicity, just draw the mesh (assuming the shader is already bound)
//        renderer.Draw(*mesh, *defaultMaterial, modelMatrix, viewProj, lightPos, lightColor, camPos);
//    }
//}

void Model::Draw(const Renderer& renderer,
    const glm::mat4& modelMatrix,
    const glm::mat4& viewProj,
    const glm::vec3& lightPos,
    const glm::vec4& lightColor,
    const glm::vec3& camPos,
    std::shared_ptr<Material> overrideMaterial)
{
    std::shared_ptr<Material> mat = nullptr;

    if (overrideMaterial)
        mat = overrideMaterial;
    else if (m_CustomMaterial)
        mat = m_CustomMaterial;
    else
        mat = m_DefaultMaterial;

    for (auto& mesh : m_Meshes)
    {
        renderer.Draw(*mesh, *mat, modelMatrix, viewProj, lightPos, lightColor, camPos);
    }
}

void Model::SetMaterial(std::shared_ptr<Material> material)
{
    m_CustomMaterial = material;
}
