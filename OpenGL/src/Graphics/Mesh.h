#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include <memory>

class Mesh
{
public:
    Mesh(const void* vertices, size_t vertByteSize,
        const GLuint* indices, unsigned int indexCount,
        const VertexBufferLayout& layout);

    void Bind() const;
    unsigned int GetIndexCount() const;

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer> m_IBO;
    unsigned int m_IndexCount;
};