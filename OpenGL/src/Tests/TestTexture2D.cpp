#include "TestTexture2D.h"

#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/VertexArray.h"
#include "../Graphics/VertexBufferLayout.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Texture.h"
#include "../vendor/imgui/ImGuiFileDialog.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Test
{
	TestTexture2D::TestTexture2D()
        : m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{
        //vertex positions
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f, //0
             50.0f, -50.0f, 1.0f, 0.0f, //1
             50.0f,  50.0f, 1.0f, 1.0f, //2
            -50.0f,  50.0f, 0.0f, 1.0f  //3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };



        // VERTEX ARRAY OBJECT
        m_VAO = std::make_unique<VertexArray>();

        // VERTEX BUFFER 
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

        // specifies the layout
        VertexBufferLayout layout;
        layout.Push<float>(2); // vertices
        layout.Push<float>(2); // tex coords
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        // INDEX BUFFER 
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        // SHADER
        m_Shader = std::make_unique<Shader>("res/shaders/Texture.shader");

        // SET projection matrix
        m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        m_Texture = std::make_unique<Texture>("res/textures/eagle.png");

        m_Shader->Bind();
        m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRenderer()
	{

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind(0);

        // object 1
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniform1i("u_Texture", 0);
            m_Shader->SetUniformMat4("u_MVP", mvp);
            // in general this function should take in va, ib, material(which has shader info,
            // plus all the data)
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

	}

    void TestTexture2D::LoadTexture(const std::string& path)
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

	void TestTexture2D::OnImGuiRenderer()
	{
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
                TestTexture2D::LoadTexture(filePathName);
            }
            // Close the dialog (important!)
            ImGuiFileDialog::Instance()->Close();
        }

        ImGui::SliderFloat3("Translation obj 1", &m_TranslationA.x, 0.0f, 960.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}