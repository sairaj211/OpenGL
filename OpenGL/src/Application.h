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
    GLFWwindow* m_Window = nullptr;
    int m_WindowWidth = 960;
    int m_WindowHeight = 540;
    float m_ImGuiWindowWidth = 960.0f;
    float m_ImGuiWindowHeight = 540.0f;

    Renderer m_Renderer;
    FrameRateController* m_FrameRateController = nullptr;
    Test::Test* m_CurrentTest = nullptr;
    Test::TestMenu* m_TestMenu = nullptr;

    CameraSettings m_CameraSettings;
    bool m_EnableVSync = false;
};