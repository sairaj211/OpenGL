#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Application.h"
#include "Graphics/Renderer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexArray.h"
#include "Graphics/VertexBufferLayout.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//imgui 
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Test Framework
#include "Tests/TestClearColor.h"
#include "Tests/TestTexture2D.h"
#include "Tests/TestBatching.h"
#include "Tests/TestObject3D.h"
#include "Tests/Lighting/TestPhongLighting.h"
#include "Tests/Lighting/TestSpecularMap.h"
#include "Tests/Lighting/TestTypesOfLights.h"

// Utils
#include "Utils/Camera.h"
#include "Utils/FrameRateController.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Camera::GetInstance().OnResize(width, height);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera::GetInstance().ProcessScroll(static_cast<float>(yoffset));
}

Application::Application()
{
    InitWindow();
    InitGL();
    InitImGui();
    SetupTests();
}

Application::~Application()
{
    Shutdown();
}

void Application::InitWindow()
{
    if (!glfwInit())
        std::exit(-1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "OpenGL Test Framework", nullptr, nullptr);
    if (!m_Window)
    {
        glfwTerminate();
        std::exit(-1);
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
    glfwSetScrollCallback(m_Window, ScrollCallback);
}

void Application::InitGL()
{
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        std::exit(-1);
    }

    m_CameraSettings = {
        m_WindowWidth, m_WindowHeight,
        glm::vec3(0.0f, 0.0f, 2.0f),
        45.0f, 0.1f, 100.0f
    };

    Camera& camera = Camera::GetInstance(m_CameraSettings);
    camera.SetWindow(m_Window);

    m_FrameRateController = FrameRateController::GetInstance();
    m_FrameRateController->SetMaxFrameRate(60.0f);
}

void Application::InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init();
}

void Application::SetupTests()
{
    m_TestMenu = new Test::TestMenu(m_CurrentTest);
    m_CurrentTest = m_TestMenu;

    m_TestMenu->RegisterTest<Test::TestClearColor>("Clear Color");
    m_TestMenu->RegisterTest<Test::TestTexture2D>("Texture 2D");
    m_TestMenu->RegisterTest<Test::TestBatching>("Batching"); 
    m_TestMenu->RegisterTest<Test::TestObject3D>("Object 3D");
    m_TestMenu->RegisterTest<Test::TestPhongLighting>("Phong Lighting");
    m_TestMenu->RegisterTest<Test::TestSpecularMap>("Specular Map");
    m_TestMenu->RegisterTest<Test::TestTypesOfLights>("Types of Lights");
}

void Application::Run()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        m_FrameRateController->FrameStart();

        m_Renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (m_CurrentTest)
        {
            float deltaTime = (float)m_FrameRateController->GetDeltaTime();
            Camera::GetInstance().HandleInputs();

            m_CurrentTest->OnUpdate(deltaTime);
            m_CurrentTest->OnRenderer();

            //ImGui::Begin("Test");
            ImGui::Begin(m_CurrentTest->GetName().c_str());
            if (m_CurrentTest != m_TestMenu && ImGui::Button("Back"))
            {
                delete m_CurrentTest;
                ResetOpenGLState();
                m_CurrentTest = m_TestMenu;
            }
            m_FrameRateController->ImGuiRender();
            m_CurrentTest->OnImGuiRenderer();
            ImGui::End();
        }

        OnImGuiRender();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_Window);
        glfwPollEvents();

        m_FrameRateController->FrameEnd();
    }
}

void Application::OnImGuiRender()
{
    const float fpsWindowHeight = 40.0f;  // Adjust if needed to match actual FPS window height
    const float spacing = 10.0f;
    ImVec2 pos(10.0f, fpsWindowHeight + spacing + 10.0f); // Below the FPS window

    //ImVec2 size(200.0f, 300.0f); // Optional fixed size

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    //	ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoMove;

    static int windowWidth = 0;
    static int windowHeight = 0;

    int currentWidth, currentHeight;
    glfwGetWindowSize(m_Window, &currentWidth, &currentHeight);

    if (currentWidth != static_cast<int>(m_ImGuiWindowWidth) ||
        currentHeight != static_cast<int>(m_ImGuiWindowHeight))
    {
        m_ImGuiWindowWidth = static_cast<float>(currentWidth);
        m_ImGuiWindowHeight = static_cast<float>(currentHeight);

        windowWidth = currentWidth;
        windowHeight = currentHeight;
    }

    ImGui::Begin("Application Settings");

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    static float targetFPS = static_cast<float>(m_FrameRateController->GetMaxFrameRate());
    if (ImGui::InputFloat("Target FPS", &targetFPS, 1.0f, 10.0f, "%.1f"))
    {
        if (targetFPS < 1.0f) targetFPS = 1.0f;
        if (targetFPS > 240.0f) targetFPS = 240.0f;
        m_FrameRateController->SetMaxFrameRate(targetFPS);
    }

    if (ImGui::Checkbox("VSync", &m_EnableVSync))
    {
        glfwSwapInterval(m_EnableVSync ? 1 : 0);
    }

    if (ImGui::InputInt("Window Width", &windowWidth))
    {
        if (windowWidth < 640) windowWidth = 640;
        if (windowWidth > 1920) windowWidth = 1920;

        m_ImGuiWindowWidth = (float)windowWidth;
        glfwSetWindowSize(m_Window, windowWidth, (int)m_ImGuiWindowHeight);
        Camera::GetInstance().OnResize(windowWidth, (int)m_ImGuiWindowHeight);
    }

    if (ImGui::InputInt("Window Height", &windowHeight))
    {
        if (windowHeight < 360) windowHeight = 360;
        if (windowHeight > 1080) windowHeight = 1080;

        m_ImGuiWindowHeight = (float)windowHeight;
        glfwSetWindowSize(m_Window, (int)m_ImGuiWindowWidth, windowHeight);
        Camera::GetInstance().OnResize((int)m_ImGuiWindowWidth, windowHeight);
    }

    ImGui::End();
}

void Application::Shutdown()
{
    delete m_CurrentTest;
    if (m_CurrentTest != m_TestMenu)
        delete m_TestMenu;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Application::ResetOpenGLState()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glBindVertexArray(0);
    glUseProgram(0);
}