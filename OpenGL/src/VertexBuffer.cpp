#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    //generate a buffer
    glGenBuffers(1, &m_RendererID);
    //use this buffer to draw and the layout for the data is in arrays
    //selecting which buffer to use to draw
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    //Now provide the data
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}
 
void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
