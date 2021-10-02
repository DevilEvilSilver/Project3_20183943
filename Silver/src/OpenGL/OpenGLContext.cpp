#include "pch.h"
#include "OpenGLContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Silver {

	OpenGLContext::OpenGLContext(GLFWwindow * window)
		: m_WindowHandle(window)
	{
		if (!window)
			SV_CORE_ERROR("window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			SV_CORE_ERROR("Failed to init Glad!");
		}
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}