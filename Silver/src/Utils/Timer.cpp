#include "pch.h"
#include "Timer.h"

#include <GLFW/glfw3.h>

namespace Silver {

	void Timer::UpdateTimestep()
	{
		m_LastFrameTime = m_NewFrameTime;
		m_NewFrameTime = (float)glfwGetTime();
	}

}