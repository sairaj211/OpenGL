#pragma once

#include "Graphics/Renderer.h"
#include "Utils/Camera.h"
#include "Utils/FrameRateController.h"

namespace Test {
    class Test;
    class TestMenu;
}

class Application
{
public:
    Application();
    ~Application();
    void Run();

    static void ResetOpenGLState();

private:
    void InitWindow();
    void InitGL();
    void InitImGui();
    void SetupTests();
    void Shutdown();
    void OnImGuiRender();
    void HandleResize();

private:
    Camera& m_Camera;

    GLFWwindow* m_Window = nullptr;
    int m_WindowWidth = 1920;
    int m_WindowHeight = 1080;
    float m_ImGuiWindowWidth = 1920.0f;
    float m_ImGuiWindowHeight = 1080.0f;

    Renderer m_Renderer;
    FrameRateController* m_FrameRateController = nullptr;
    Test::Test* m_CurrentTest = nullptr;
    Test::TestMenu* m_TestMenu = nullptr;

    CameraSettings m_CameraSettings;
    bool m_EnableVSync = false;
};