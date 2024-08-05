#include <cstdint>

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "VertexArray.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i) 
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));

		// Convert unsigned int to uintptr_t
		uintptr_t offset_ptr = static_cast<uintptr_t>(offset);

		/*uintptr_t: This type is large enough to hold a pointer, 
		ensuring that you don't lose information during the conversion.
		reinterpret_cast: This cast is used to convert between uintptr_t
		and const void*.*/

		// Convert uintptr_t to const void*
		const void* ptr = reinterpret_cast<const void*>(offset_ptr);

		// specifies the layout
		GLCall(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), ptr));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

	//	for reference 
	//	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
