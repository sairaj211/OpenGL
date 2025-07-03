#include "Camera.h"
#include <imgui/imgui.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

Camera::Camera(const CameraSettings& settings)
    : m_Width(settings.width),
      m_Height(settings.height),
      m_Position(settings.position),
      m_FOVdeg(settings.fov),
      m_NearPlane(settings.nearPlane),
      m_FarPlane(settings.farPlane)
{
}

Camera& Camera::GetInstance(const CameraSettings& settings)
{
    static Camera instance(settings);
    return instance;
}

void Camera::SetWindow(GLFWwindow* window)
{
    m_Window = window;
}

glm::mat4 Camera::CalculateMatrix() const
{
    glm::mat4 view = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
    glm::mat4 projection = glm::perspective(glm::radians(m_FOVdeg), (float)m_Width / m_Height, m_NearPlane, m_FarPlane);
    return projection * view;
}

void Camera::HandleInputs()
{
    if (!m_Window || ImGui::GetIO().WantCaptureMouse)
        return;

    HandleKeyboardInput();
    HandleMouseInput();
}

void Camera::HandleKeyboardInput()
{
    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        m_Position += m_Speed * m_Orientation;
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
        m_Position -= m_Speed * m_Orientation;
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
        m_Position -= glm::normalize(glm::cross(m_Orientation, m_Up)) * m_Speed;
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
        m_Position += glm::normalize(glm::cross(m_Orientation, m_Up)) * m_Speed;
    if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_Position += m_Speed * m_Up;
    if (glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_Position -= m_Speed * m_Up;

    if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_Speed = 0.3f;
    else
        m_Speed = 0.1f;
}

void Camera::HandleMouseInput()
{
    double mouseX, mouseY;
    glfwGetCursorPos(m_Window, &mouseX, &mouseY);

    // Check if LMB is being held
    if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (!m_IsDragging)
        {
            // First frame of the drag — just record position and return
            m_LastMouseX = mouseX;
            m_LastMouseY = mouseY;
            m_IsDragging = true;
            return; // Prevents a sudden camera snap
        }

        // Calculate movement delta
        float deltaX = static_cast<float>(mouseX - m_LastMouseX);
        float deltaY = static_cast<float>(mouseY - m_LastMouseY);

        // Update last known position
        m_LastMouseX = mouseX;
        m_LastMouseY = mouseY;

        // Convert movement to rotation amounts
        float rotX = m_Sensitivity * deltaY / m_Height;
        float rotY = m_Sensitivity * deltaX / m_Width;

        // Compute right axis for vertical rotation
        glm::vec3 right = glm::normalize(glm::cross(m_Orientation, m_Up));

        // Try rotating up/down
        glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX), right);

        // Clamp pitch to avoid flipping over
        if (std::abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            m_Orientation = newOrientation;
        }

        // Rotate left/right around world up
        m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);
    }
    else
    {
        // Mouse not held — stop dragging
        m_IsDragging = false;
    }

    // RIGHT MOUSE BUTTON: PAN
    if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (!m_IsPanning)
        {
            m_LastMouseX = mouseX;
            m_LastMouseY = mouseY;
            m_IsPanning = true;
            return;
        }

        float deltaX = static_cast<float>(mouseX - m_LastMouseX);
        float deltaY = static_cast<float>(mouseY - m_LastMouseY);

        m_LastMouseX = mouseX;
        m_LastMouseY = mouseY;

        float panSpeed = 0.005f * m_Speed; // You can expose this to ImGui too

        glm::vec3 right = glm::normalize(glm::cross(m_Orientation, m_Up));
        glm::vec3 up = m_Up;

        // Pan camera position
        m_Position -= right * deltaX * panSpeed;
        m_Position += up * deltaY * panSpeed;
    }
    else
    {
        m_IsPanning = false;
    }
}

void Camera::ImGuiRender()
{
    if (ImGui::CollapsingHeader("Camera Settings"))
    {
        ImGui::DragFloat3("Position", &m_Position.x, 0.1f);
        ImGui::DragFloat3("Orientation", &m_Orientation.x, 0.1f);
        ImGui::DragFloat("FOV", &m_FOVdeg, 0.1f, 1.0f, 180.0f);
        ImGui::DragFloat("Sensitivity", &m_Sensitivity, 0.1f, 1.0f, 100.0f);
        ImGui::DragFloat("Scroll Sensitivity", &m_ScrollSensitivity, 0.1f, 0.1f, 10.0f);
    }
}

void Camera::ProcessScroll(float yoffset)
{
    m_FOVdeg -= yoffset * m_ScrollSensitivity;  // Apply sensitivity multiplier  // Zoom in/out by changing FOV
    if (m_FOVdeg < 1.0f) m_FOVdeg = 1.0f;
    if (m_FOVdeg > 90.0f) m_FOVdeg = 90.0f;
}
