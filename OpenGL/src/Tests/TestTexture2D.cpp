#include "TestTexture2D.h"



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

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");

        // SET projection matrix
        m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        m_Texture = std::make_unique<Texture>("res/textures/eagle.png");

        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.6f, 0.0f, 0.7f, 1.0f);
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
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();

        // object 1
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniform4f("u_Color", 0.1f, 0.0f, 0.7f, 1.0f);
            m_Shader->SetUniformMat4("u_MVP", mvp);
            // in general this function should take in va, ib, material(which has shader info,
            // plus all the data)
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        // object 2
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniform4f("u_Color", 0.1f, 0.0f, 0.7f, 1.0f);
            m_Shader->SetUniformMat4("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
	}

	void TestTexture2D::OnImGuiRenderer()
	{
        ImGui::SliderFloat3("Translation obj 1", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation obj 2", &m_TranslationB.x, 0.0f, 960.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}