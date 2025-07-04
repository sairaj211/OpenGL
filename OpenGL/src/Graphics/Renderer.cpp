#include "Renderer.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include <iostream>


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
            " " << file << ": " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const Mesh& mesh, const Material& mat, const glm::mat4& model, 
    const glm::mat4& viewProj, const glm::vec3& lightPos, const glm::vec4& lightColor, const glm::vec3& camPos) const
{
    mat.Bind();
    auto shader = mat.GetShader();  
    shader->SetUniformMat4("model", model);
    shader->SetUniformMat4("camMatrix", viewProj);
    shader->SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
    shader->SetUniform4f("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shader->SetUniform3f("camPos", camPos.x, camPos.y, camPos.z);

    mesh.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawLight(const Mesh& mesh, const Material& mat, const glm::mat4& model,
    const glm::mat4& viewProj, const glm::vec4& lightColor) const
{
    mat.Bind();
    auto shader = mat.GetShader();
    shader->SetUniformMat4("model", model);
    shader->SetUniformMat4("camMatrix", viewProj);
    shader->SetUniform4f("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);

    mesh.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::SetClearColor() const
{
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
}
