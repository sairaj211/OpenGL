#pragma once

#include "Test.h"
#include "../Utils/Camera.h"
#include <memory>
#include <string>

class Renderer;
class Model;

namespace Test
{
	class TestLoadModel : public Test
	{
	public:
		TestLoadModel();
		~TestLoadModel();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;

		const std::string& GetName() const override;

	private:
		Camera& m_Camera = Camera::GetInstance();

		std::unique_ptr<Model> m_Model;
		std::unique_ptr<Renderer> m_Renderer;

		glm::mat4 m_ModelMatrix, m_PlaneModelMatrix;
		glm::vec3 m_LightPos = glm::vec3(0.0f, 3.0f, 3.0f);
		glm::vec4 m_LightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		std::string m_Name = "Load Model Test";

		std::unique_ptr<Mesh> m_Plane;

		std::shared_ptr<Material> m_SharedMaterial;
	};
}