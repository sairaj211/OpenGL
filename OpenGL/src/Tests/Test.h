#pragma once

#include "../Graphics/Renderer.h"
#include "imgui/imgui.h"
#include <iostream>
#include <vector>
#include <functional>

namespace Test
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime){}
		virtual void OnRenderer(){}
		virtual void OnImGuiRenderer(){}

		virtual const std::string& GetName()const = 0;
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);
		~TestMenu() {}

		void OnImGuiRenderer() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []()
			{
				return new T();
			}));
		}

		const std::string& GetName()const override;
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};
}