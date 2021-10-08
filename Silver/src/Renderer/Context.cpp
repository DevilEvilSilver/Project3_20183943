#include "pch.h"
#include "Context.h"

#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace Silver {

	Context::Context(GLFWwindow * window)
		: m_WindowHandle(window)
	{
		if (!window)
			SV_CORE_ERROR("window handle is null!");
	}

	void Context::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			SV_CORE_ERROR("Failed to init Glad!");
		}

		SV_CORE_INFO("OpenGL info:");
		SV_CORE_INFO("	Vendor: {0}", glGetString(GL_VENDOR));
		SV_CORE_INFO("	Renderer: {0}", glGetString(GL_RENDERER));
		SV_CORE_INFO("	Version: {0}", glGetString(GL_VERSION));
	}

	void Context::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}