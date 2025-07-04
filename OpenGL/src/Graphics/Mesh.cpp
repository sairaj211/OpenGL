#include "Mesh.h"

Mesh::Mesh(const void* vertices, size_t vertByteSize,
    const GLuint* indices, unsigned int indexCount,
    const VertexBufferLayout& layout)
    : m_IndexCount(indexCount)
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(vertices, vertByteSize);
    m_IBO = std::make_unique<IndexBuffer>(indices, indexCount);
    m_VAO->AddBuffer(*m_VBO, layout);
}

void Mesh::Bind() const
{
    m_VAO->Bind();
    m_IBO->Bind();
}

unsigned int Mesh::GetIndexCount() const
{
    return m_IndexCount;
}