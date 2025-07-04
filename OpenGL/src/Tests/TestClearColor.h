#pragma once

#include "test.h"

namespace Test
{
	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;
		const std::string& GetName()const override;

	private :
		float m_ClearColor[4];
		std::string m_Name = "Clear Color Test";
	};
}