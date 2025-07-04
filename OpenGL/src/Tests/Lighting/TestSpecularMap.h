#pragma once

#include "../Test.h"
#include "../Utils/Camera.h"

class Texture;
class Shader;
class VertexBuffer;
class IndexBuffer;
class VertexArray;

namespace Test
{
	class TestSpecularMap : public Test
	{
	public:
		TestSpecularMap();
		~TestSpecularMap();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;

		void LoadTexture(const std::string& path);

		const std::string& GetName()const override;
	private:
		Camera& m_Camera = Camera::GetInstance();
		 
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Texture> m_Texture1;
		std::string m_TexturePath = "res/textures/brick.jpeg";

		std::unique_ptr<VertexArray> m_VAOLight;
		std::unique_ptr<IndexBuffer> m_IndexBufferLight;
		std::unique_ptr<VertexBuffer> m_VertexBufferLight;
		std::unique_ptr<Shader> m_ShaderLight;

		glm::mat4 m_Proj, m_View, m_Model;
		glm::mat4 m_ProjLight, m_ViewLight, m_ModelLight;

		glm::vec3 m_LightPos = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec4 m_LightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		bool m_ShowFileDialog = false;
		float m_Rotation;

		std::string m_Name = "SpecularMap Test";
	};
}