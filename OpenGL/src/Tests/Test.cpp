#include "Test.h"
#include <GLFW/glfw3.h>

namespace Test
{
	TestMenu::TestMenu(Test*& currentTestPointer)
		:m_CurrentTest(currentTestPointer)
	{
	}

	void TestMenu::OnImGuiRenderer()
	{
        int screenWidth, screenHeight;
        glfwGetWindowSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);

        ImVec2 padding(10.0f, 10.0f);
        ImVec2 windowSize(200.0f, 300.0f); // You can adjust this to fit your layout

        // Top-right corner position = (screenWidth - window width - padding)
        ImVec2 position(screenWidth - windowSize.x - padding.x, padding.y);

        // Set window position and size
        ImGui::SetNextWindowPos(position, ImGuiCond_Always);
        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoNav;

        ImGui::Begin("Test Menu", nullptr, flags);

        for (auto& test : m_Tests)
        {
            if (ImGui::Button(test.first.c_str()))
            {
                m_CurrentTest = test.second();
            }
        }

        ImGui::End();
	}
    const std::string& TestMenu::GetName()const
    {
        // TODO: insert return statement here
        static std::string name = "Test Menu";
        return name;
    }
}