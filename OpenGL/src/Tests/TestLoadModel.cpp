#include "TestLoadModel.h"
#include "../Graphics/Model.h"
#include "../Graphics/Renderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace Test
{
	TestLoadModel::TestLoadModel()
	{
		Vertex planeVertices[] = {
			//           position                      normal                   color                  texUV
			{ glm::vec3(-1.25f, 0.0f, 1.25f),  glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.8f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec3(1.25f, 0.0f,  1.25f),  glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.8f), glm::vec2(1.0f, 0.0f) },
			{ glm::vec3(1.25f, 0.0f, -1.25f),  glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.8f), glm::vec2(1.0f, 1.0f) },
			{ glm::vec3(-1.25f, 0.0f, -1.25f), glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec3(0.8f), glm::vec2(0.0f, 1.0f) }
		};

		GLuint planeIndices[] = {
			0, 1, 2,
			2, 3, 0	
		};

		// Vertex layout: same as your other meshes
		VertexBufferLayout planeLayout;
		planeLayout.Push<float>(3); // position
		planeLayout.Push<float>(3); // normal
		planeLayout.Push<float>(3); // color
		planeLayout.Push<float>(2); // texUV

		m_Plane = std::make_unique<Mesh>(
			planeVertices,
			sizeof(planeVertices),
			planeIndices,
			sizeof(planeIndices) / sizeof(GLuint),
			planeLayout
		);

		// Load the model (adjust path as needed)
		m_Model = std::make_unique<Model>("res/models/teapot.obj");

		float scaleFactor = 0.1f; // or whatever fits your scene
		m_ModelMatrix = glm::mat4(1.0f);
		m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(scaleFactor));

		m_Renderer = std::make_unique<Renderer>();

		// --- Create shared shader and material ---
		auto shader = std::make_shared<Shader>("res/shaders/default.shader");
		m_SharedMaterial = std::make_shared<Material>(shader);

		m_PlaneModelMatrix = glm::mat4(1.0f);
		m_PlaneModelMatrix = glm::translate(m_PlaneModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
	}

	TestLoadModel::~TestLoadModel()
	{
	}

	void TestLoadModel::OnUpdate(float deltaTime)
	{
	}

	void TestLoadModel::OnRenderer()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 viewProj = m_Camera.CalculateMatrix();
		glm::vec3 camPos = m_Camera.GetCameraPosition();

		m_Model->Draw(*m_Renderer, m_ModelMatrix, viewProj, m_LightPos, m_LightColor, camPos);

		// Draw plane explicitly with the same material
		m_Renderer->Draw(*m_Plane, *m_SharedMaterial, m_PlaneModelMatrix, viewProj, m_LightPos, m_LightColor, camPos);

		glDisable(GL_DEPTH_TEST);
	}

	void TestLoadModel::OnImGuiRenderer()
	{
		ImGui::SliderFloat3("Light Position", &m_LightPos.x, -10.0f, 10.0f);
		ImGui::ColorEdit3("Light Color", &m_LightColor.x);
	}

	const std::string& TestLoadModel::GetName() const
	{
		return m_Name;
	}
}