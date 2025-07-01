#include "TestBatching.h"



namespace Test
{
    TestBatching::TestBatching()
        : m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
    {
        //vertex positions
        float positions[] = {
            -150.0f, -50.0f, 0.18f, 0.6f, 0.96f, 1.0f, //0
            -50.0f,  -50.0f, 0.18f, 0.6f, 0.96f, 1.0f, //1
            -50.0f,   50.0f, 0.18f, 0.6f, 0.96f, 1.0f, //2
            -150.0f,  50.0f, 0.18f, 0.6f, 0.96f, 1.0f, //3

             50.0f,  -50.0f, 1.0f, 0.93f, 0.24f, 1.0f, //4
             150.0f, -50.0f, 1.0f, 0.93f, 0.24f, 1.0f, //5
             150.0f,  50.0f, 1.0f, 0.93f, 0.24f, 1.0f, //6
             50.0f,   50.0f, 1.0f, 0.93f, 0.24f, 1.0f  //7
        };

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

      

        // VERTEX ARRAY OBJECT
        m_VAO = std::make_unique<VertexArray>();

        // VERTEX BUFFER 
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 6 * sizeof(float));

        // specifies the layout
        VertexBufferLayout layout;
        layout.Push<float>(2); // vertices
        layout.Push<float>(4); // vertex volor
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        // INDEX BUFFER 
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

        // SHADER
        m_Shader = std::make_unique<Shader>("res/shaders/Batching.shader");

        // SET projection matrix
        m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        m_Shader->Bind();
    }

    TestBatching::~TestBatching()
    {
    }

    void TestBatching::OnUpdate(float deltaTime)
    {
    }

    void TestBatching::OnRenderer()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
        glm::mat4 mvp = m_Proj * m_View * model;
        m_Shader->Bind();
        m_Shader->SetUniformMat4("u_MVP", mvp);

        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

    }

    void TestBatching::OnImGuiRenderer()
    {
        ImGui::SliderFloat3("Translation obj 1", &m_TranslationA.x, 0.0f, 960.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}