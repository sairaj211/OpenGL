#pragma once

#include "test.h"

class Texture;
class Shader;
class VertexBuffer;
class IndexBuffer;
class VertexArray;

namespace Test
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;

		void LoadTexture(const std::string& path);
		const std::string& GetName()const override;

	private :
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::string m_TexturePath = "res/textures/eagle.jpeg";

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA;
		glm::vec3 m_TranslationB;

		std::string m_Name = "Texture2D Test";
	};
}