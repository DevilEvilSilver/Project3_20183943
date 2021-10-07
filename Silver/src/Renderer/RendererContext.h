#pragma once

struct GLFWwindow;

namespace Silver {

	class RendererContext
	{
	public:
		RendererContext(GLFWwindow* window);
		~RendererContext() = default;

		void Init();
		void SwapBuffers();

	private:
		GLFWwindow* m_WindowHandle;
	};

}