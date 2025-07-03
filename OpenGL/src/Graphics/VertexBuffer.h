#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal; // You can initialize this to {0, 1, 0} for now
	glm::vec3 color;
	glm::vec2 texUV;
};

class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};