#pragma once
#include "GLFW/glfw3.h"

class TimeProvider
{
public:
	TimeProvider()
		: m_lastTime(GetTime())
	{
	}

	float GetDeltaTime()
	{
		float now = GetTime();
		float dt = now - m_lastTime;
		m_lastTime = now;
		return dt;
	}

	static float GetTime()
	{
		return static_cast<float>(glfwGetTime());
	}

private:
	float m_lastTime = 0;
};