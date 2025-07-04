#pragma once

#include "test.h"
#include "../Utils/Camera.h"

class Texture;
class Shader;
class VertexBuffer;
class IndexBuffer;
class VertexArray;


namespace Test
{
	class TestObject3D : public Test
	{
	public:
		TestObject3D();
		~TestObject3D();

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
		std::string m_TexturePath = "res/textures/brick.jpeg";

		glm::mat4 m_Proj, m_View, m_Model;

		bool m_ShowFileDialog = false;
		float m_Rotation;
		float m_RotationSpeed = 50.0f; // degrees per second (adjust as needed)
		std::string m_Name = "Object3D Test";
	};
}