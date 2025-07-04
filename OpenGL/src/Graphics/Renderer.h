#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Mesh;
class Material;

class Renderer
{
public:
    // to draw we need va(vb is bound to it), ib and shader
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

    // New overload
    void Draw(const Mesh& mesh, const Material& mat,
        const glm::mat4& model, const glm::mat4& viewProj,
        const glm::vec3& lightPos, const glm::vec4& lightColor,
        const glm::vec3& camPos) const;

    // for light
    void DrawLight(const Mesh& mesh, const Material& mat, const glm::mat4& model,
        const glm::mat4& viewProj, const glm::vec4& lightColor) const;

    void Clear() const;
    void SetClearColor() const;
};