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


void Application::ResetOpenGLState()
{
    // General safe defaults
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // If you're using a default VAO or binding 0
    glBindVertexArray(0);
    glUseProgram(0);
}


int main()
{
    // Initialize GLFW
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(960, 540, "OpenGL Test Framework", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1); // Enable vsync optional

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // Core Rendering Objects
    Renderer renderer;

    // Setup Test Framework
    Test::Test* currentTest = nullptr;
    Test::TestMenu* testMenu = new Test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<Test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<Test::TestTexture2D>("Texture 2D");
    testMenu->RegisterTest<Test::TestBatching>("Batching");
    testMenu->RegisterTest<Test::TestObject3D>("Object 3D");

    // Timing
    float lastFrameTime = glfwGetTime();

    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        renderer.SetClearColor();
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentTest)
        {
            currentTest->OnUpdate(deltaTime);
            currentTest->OnRenderer();

            ImGui::Begin("Test");
            if (currentTest != testMenu && ImGui::Button("Back"))
            {
                delete currentTest;
                Application::ResetOpenGLState();
                currentTest = testMenu;
            }
            currentTest->OnImGuiRenderer();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    delete currentTest;
    if (currentTest != testMenu)
    {
        delete testMenu;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
