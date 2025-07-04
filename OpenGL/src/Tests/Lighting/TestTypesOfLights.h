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
	class TestTypesOfLights : public Test
	{
	public:
		TestTypesOfLights();
		~TestTypesOfLights();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;

		void LoadTexture(const std::string& path);
		const std::string& GetName()const override;
	private:
		Camera& m_Camera = Camera::GetInstance();

		glm::mat4 m_Proj, m_View, m_PlaneModel;
		glm::mat4 m_ProjLight, m_ViewLight, m_ModelLight;

		glm::vec3 m_LightPos = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec4 m_LightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		bool m_ShowFileDialog = false;
		float m_Rotation;
		
		int m_LightType = 0; // 0 = directional, 1 = point, 2 = spot

		std::string m_Name = "TypesOfLights Test";

		std::unique_ptr<Mesh> m_PlaneMesh;
		std::unique_ptr<Material> m_PlaneMat;
		std::unique_ptr<Mesh> m_LightMesh;
		std::unique_ptr<Material> m_LightMat;
		std::unique_ptr<Renderer> m_Renderer;
	};
}