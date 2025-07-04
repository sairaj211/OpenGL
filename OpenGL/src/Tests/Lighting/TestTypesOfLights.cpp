#include "TestTypesOfLights.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/VertexArray.h"
#include "../Graphics/VertexBufferLayout.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "../vendor/imgui/ImGuiFileDialog.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

namespace Test
{
	TestTypesOfLights::TestTypesOfLights()
	{
		Vertex vertices[] = {
			//           position						normal                       color					texUV
			{ glm::vec3(-1.f, 0.0f,  1.f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec3(-1.f, 0.0f, -1.f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.0f, 1.0f) },
			{ glm::vec3(1.5f, 0.0f, -1.f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 1.0f) },
			{ glm::vec3(1.5f, 0.0f,  1.f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.0f, 0.0f) }
		};

		// Indices for vertices order
		GLuint indices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		// VERTEX ARRAY OBJECT
		m_VAO = std::make_unique<VertexArray>();

		// VERTEX BUFFER 
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

		// specifies the layout
		VertexBufferLayout layout;
		layout.Push<float>(3); // position
		layout.Push<float>(3); // normal
		layout.Push<float>(3); // color
		layout.Push<float>(2); // texUV
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		// INDEX BUFFER 
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, static_cast<unsigned int>(sizeof(indices) / sizeof(indices[0])));

		// SHADER
		m_Shader = std::make_unique<Shader>("res/shaders/TypesOfLights.shader");

		// SET projection matrix
		m_Proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		glm::vec3 plane = glm::vec3(0.0f, -0.5f, 0.0f);
		glm::mat4 planeModel = glm::mat4(1.0f);
		m_Model = glm::translate(planeModel, plane);

		m_Texture = std::make_unique<Texture>("res/textures/planks.png");
		m_Texture1 = std::make_unique<Texture>("res/textures/planksSpec.png");

		glm::mat4 lightModel = glm::mat4(1.0f);

		m_Shader->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);
		m_Shader->SetUniform1i("u_Texture1", 1);

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

		// VERTEX ARRAY OBJECT
		m_VAOLight = std::make_unique<VertexArray>();

		// VERTEX BUFFER 
		m_VertexBufferLight = std::make_unique<VertexBuffer>(lightVertices, sizeof(lightVertices));

		// specifies the layout
		VertexBufferLayout layoutLight;
		layoutLight.Push<float>(3); // vertices

		m_VAOLight->AddBuffer(*m_VertexBufferLight, layoutLight);

		// INDEX BUFFER 
		m_IndexBufferLight = std::make_unique<IndexBuffer>(lightIndices, static_cast<unsigned int>(sizeof(lightIndices) / sizeof(lightIndices[0])));

		// SHADER
		m_ShaderLight = std::make_unique<Shader>("res/shaders/Light.shader");

		// SET projection matrix
		m_ProjLight = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
		m_ViewLight = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		m_ModelLight = glm::translate(lightModel, m_LightPos);

		m_ShaderLight->Bind();
	}

	TestTypesOfLights::~TestTypesOfLights()
	{
	}

	void TestTypesOfLights::OnUpdate(float deltaTime)
	{
	}

	void TestTypesOfLights::OnRenderer()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		Renderer renderer;

		m_Texture->Bind(0);
		m_Texture1->Bind(1);
		m_Shader->Bind();

		glm::mat4 mvp = m_Camera.CalculateMatrix();

		m_Shader->SetUniform1i("u_LightType", m_LightType);
		m_Shader->SetUniformMat4("model", m_Model);
		m_Shader->SetUniformMat4("camMatrix", mvp);
		m_Shader->SetUniform3f("lightPos", m_LightPos.x, m_LightPos.y, m_LightPos.z);
		m_Shader->SetUniform4f("lightColor", m_LightColor.x, m_LightColor.y, m_LightColor.z, m_LightColor.w);

		glm::vec3 camPos = m_Camera.GetCameraPosition();
		m_Shader->SetUniform3f("camPos", camPos.x, camPos.y, camPos.z);

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

		Renderer rendererLight;
		m_ShaderLight->Bind();

		m_ModelLight = glm::translate(glm::mat4(1.0f), m_LightPos);
		m_ShaderLight->SetUniformMat4("model", m_ModelLight);
		m_ShaderLight->SetUniformMat4("camMatrix", mvp);
		m_ShaderLight->SetUniform4f("lightColor", m_LightColor.x, m_LightColor.y, m_LightColor.z, m_LightColor.w);

		rendererLight.Draw(*m_VAOLight, *m_IndexBufferLight, *m_ShaderLight);

		glDisable(GL_DEPTH_TEST);
	}

	void TestTypesOfLights::OnImGuiRenderer()
	{
		ImGui::SliderFloat3("Light Position", &m_LightPos.x, -5.0f, 5.0f);

		ImGui::ColorEdit3("Light Color", &m_LightColor.x);  // RGB editor

		const char* lightTypes[] = { "Directional", "Point", "Spot" };
		ImGui::Combo("Light Type", &m_LightType, lightTypes, IM_ARRAYSIZE(lightTypes));
	}

	const std::string& TestTypesOfLights::GetName()const
	{
		return m_Name;
	}
}