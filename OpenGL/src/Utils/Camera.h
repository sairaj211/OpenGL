#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct CameraSettings
{
    int width = 1920;
    int height = 1080;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
};

class Camera
{
public:
    static Camera& GetInstance(const CameraSettings& settings = CameraSettings());

    // Delete copy and move semantics
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(Camera&&) = delete;

    void SetWindow(GLFWwindow* window);
    void HandleInputs();
    glm::mat4 CalculateMatrix() const;
    void ImGuiRender();
    void ProcessScroll(float yoffset);
    void OnResize(int width, int height);
    glm::vec3 GetCameraPosition() { return m_Position; }

private:
    Camera(const CameraSettings& settings);

    void HandleKeyboardInput();
    void HandleMouseInput();

private:
    GLFWwindow* m_Window = nullptr;

    glm::vec3 m_Position;
    glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

    int m_Width;
    int m_Height;

    float m_ScrollSensitivity = 1.0f;  // Default scroll speed factor
    float m_Speed = 0.1f;
    float m_Sensitivity = 25.0f;
    float m_FOVdeg;
    float m_NearPlane;
    float m_FarPlane;

    bool m_IsDragging = false;
    bool m_IsPanning = false;
    double m_LastMouseX = 0.0, m_LastMouseY = 0.0;
};
