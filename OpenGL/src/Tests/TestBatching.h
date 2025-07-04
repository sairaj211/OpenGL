#pragma once

#include "test.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/VertexArray.h"
#include "../Graphics/VertexBufferLayout.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Test
{
	class TestBatching : public Test
	{
	public:
		TestBatching();
		~TestBatching();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;
		const std::string& GetName()const override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Shader> m_Shader;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA;
		glm::vec3 m_TranslationB;

		std::string m_Name = "Batching Test";
	};
}