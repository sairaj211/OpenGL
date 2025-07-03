#include "TestObject3D.h"

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
	TestObject3D::TestObject3D()
	{
		// Vertices coordinates
		float vertices[] =
		{ //     COORDINATES     /        COLORS      /   TexCoord  //
			-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
			-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
			 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
			 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
			 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
		};

		// Indices for vertices order
		unsigned int indices[] =
		{
			0, 1, 2,
			0, 2, 3,
			0, 1, 4,
			1, 2, 4,
			2, 3, 4,
			3, 0, 4
		};

		// Enables the Depth Buffer


		// VERTEX ARRAY OBJECT
		m_VAO = std::make_unique<VertexArray>();

		// VERTEX BUFFER 
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 8 * 5 * sizeof(float));

		// specifies the layout
		VertexBufferLayout layout;
		layout.Push<float>(3); // vertices
		layout.Push<float>(3); // color
		layout.Push<float>(2); // tex coords
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		// INDEX BUFFER 
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 18);

		// SHADER
		m_Shader = std::make_unique<Shader>("res/shaders/Object3D.shader");


		// SET projection matrix
		m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		m_Texture = std::make_unique<Texture>("res/textures/brick.jpeg");

		m_Shader->Bind();
		//m_Shader->SetUniform4f("u_Color", 0.6f, 0.0f, 0.7f, 1.0f);
		m_Shader->SetUniform1i("u_Texture", 0);

		m_Rotation = 0.f;
	}

	TestObject3D::~TestObject3D()
	{
	}

	void TestObject3D::OnUpdate(float deltaTime)
	{
		// Rotate by degrees per second multiplied by deltaTime for smooth animation

		m_Rotation += m_RotationSpeed * deltaTime;
	}

	void TestObject3D::OnRenderer()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_DEPTH_TEST);

		Renderer renderer;

		m_Texture->Bind();

		// Initializes matrices so they are not the null matrix
		//m_Model = glm::mat4(1.0f);
	//	m_View = glm::mat4(1.0f);
	//	m_Proj = glm::mat4(1.0f);

		// Assigns different transformations to each matrix
		//m_Model = glm::rotate(m_Model, glm::radians(m_Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	//	m_View = glm::translate(m_View, glm::vec3(0.0f, -0.5f, -2.0f));
		//m_Proj = glm::perspective(glm::radians(45.0f), (float)960 / 540, 0.1f, 100.0f);

		m_Shader->Bind();

		glm::mat4 mvp = m_Camera.CalculateMatrix();

		m_Shader->SetUniformMat4("u_MVP", mvp);

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

		glDisable(GL_DEPTH_TEST);
	}

	void TestObject3D::LoadTexture(const std::string& path)
	{
		try
		{
			m_Texture = std::make_unique<Texture>(path);
			m_TexturePath = path;
		}
		catch (const std::exception& e)
		{
			std::cerr << "Failed to load texture: " << e.what() << std::endl;
		}
	}

	void TestObject3D::OnImGuiRenderer()
	{
		m_Camera.ImGuiRender();

		ImGui::Text("Current Texture: %s", m_TexturePath.c_str());

		if (ImGui::Button("Open Texture File"))
		{
			// Open file dialog - you can specify filters if you want
			IGFD::FileDialogConfig cfg;
			cfg.path = ".";          // start in current directory
			cfg.fileName = "";       // no default file name

			ImGuiFileDialog::Instance()->OpenDialog(
				"ChooseTextureFile",
				"Choose Texture",
				".png,.jpg,.jpeg,.bmp",
				cfg
			);
		}

		// Display the file dialog
		if (ImGuiFileDialog::Instance()->Display("ChooseTextureFile"))
		{
			// Check if user has selected a file
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				// Load new texture
				TestObject3D::LoadTexture(filePathName);
			}
			// Close the dialog (important!)
			ImGuiFileDialog::Instance()->Close();
		}

		ImGui::SliderFloat("Rotation Speed (deg/s)", &m_RotationSpeed, 0.0f, 360.0f);
	}

}
