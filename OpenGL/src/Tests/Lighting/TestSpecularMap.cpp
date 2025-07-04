#include "TestSpecularMap.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/VertexArray.h"
#include "../Graphics/VertexBufferLayout.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Material.h"
#include "../Graphics/Mesh.h"
#include "../vendor/imgui/ImGuiFileDialog.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

namespace Test
{
	TestSpecularMap::TestSpecularMap()
	{
		Vertex planeVertices[] = {
			//           position						normal                       color					texUV
			{ glm::vec3(-1.f, 0.0f,  1.f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec3(-1.f, 0.0f, -1.f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 1.0f) },
			{ glm::vec3(1.5f, 0.0f, -1.f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 1.0f) },
			{ glm::vec3(1.5f, 0.0f,  1.f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 0.0f) }
		};

		// Indices for vertices order
		GLuint planeIndices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		// specifies the layout
		VertexBufferLayout planeLayout;
		planeLayout.Push<float>(3); // position
		planeLayout.Push<float>(3); // normal
		planeLayout.Push<float>(3); // color
		planeLayout.Push<float>(2); // texUV

		unsigned int indicesCount = static_cast<unsigned int>(sizeof(planeIndices) / sizeof(planeIndices[0]));
		m_PlaneMesh = std::make_unique<Mesh>(planeVertices, sizeof(planeVertices), planeIndices, indicesCount, planeLayout);

		auto planeShader = std::make_shared<Shader>("res/shaders/SpecularMap.shader");
		m_PlaneMat = std::make_unique<Material>(planeShader);

		// Load textures ONCE here and keep in variables
		auto diffuse = std::make_shared<Texture>("res/textures/planks.png");
		auto specular = std::make_shared<Texture>("res/textures/planksSpec.png");

		// Set textures in material using variables
		m_PlaneMat->SetTexture("diffuseTex", diffuse, 0);
		m_PlaneMat->SetTexture("specularTex", specular, 1);

		m_PlaneModel = glm::translate(glm::mat4(1.f), glm::vec3(0, -0.5f, 0));


		//////////////////////////////////////////////////////////////////////////////////////////////////
		// LIGHT 
		//////////////////////////////////////////////////////////////////////////////////////////////////

		GLfloat lightVertices[] =
		{ //     COORDINATES     //
			-0.1f, -0.1f,  0.1f,
			-0.1f, -0.1f, -0.1f,
			 0.1f, -0.1f, -0.1f,
			 0.1f, -0.1f,  0.1f,
			-0.1f,  0.1f,  0.1f,
			-0.1f,  0.1f, -0.1f,
			 0.1f,  0.1f, -0.1f,
			 0.1f,  0.1f,  0.1f
		};

		GLuint lightIndices[] =
		{
			0, 1, 2,
			0, 2, 3,
			0, 4, 7,
			0, 7, 3,
			3, 7, 6,
			3, 6, 2,
			2, 6, 5,
			2, 5, 1,
			1, 5, 4,
			1, 4, 0,
			4, 5, 6,
			4, 6, 7
		};

		// specifies the layout
		VertexBufferLayout layoutLight;
		layoutLight.Push<float>(3); // vertices

		unsigned int indicesCount1 = static_cast<unsigned int>(sizeof(lightIndices) / sizeof(lightIndices[0]));
		m_LightMesh = std::make_unique<Mesh>(lightVertices, sizeof(lightVertices), lightIndices, indicesCount1, layoutLight);
		auto lightShader = std::make_shared<Shader>("res/shaders/Light.shader");
		m_LightMat = std::make_unique<Material>(lightShader);
	}

	TestSpecularMap::~TestSpecularMap()
	{
	}

	void TestSpecularMap::OnUpdate(float deltaTime)
	{
	}

	void TestSpecularMap::OnRenderer()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		 // Draw plane
		glm::mat4 mvp = m_Camera.CalculateMatrix();
		glm::vec3 camPos = m_Camera.GetCameraPosition();

		m_Renderer->Draw(*m_PlaneMesh, *m_PlaneMat, m_PlaneModel, mvp, m_LightPos, m_LightColor, camPos);

		// Light cube
		m_ModelLight = glm::translate(glm::mat4(1.0f), m_LightPos);
		m_Renderer->DrawLight(*m_LightMesh, *m_LightMat, m_ModelLight, mvp, m_LightColor);

		glDisable(GL_DEPTH_TEST);
	}

	void TestSpecularMap::OnImGuiRenderer()
	{
		ImGui::SliderFloat3("Light Position", &m_LightPos.x, -5.0f, 5.0f);

		ImGui::ColorEdit3("Light Color", &m_LightColor.x);  // RGB editor
	}

	const std::string& TestSpecularMap::GetName()const
	{
		return m_Name;
	}
}