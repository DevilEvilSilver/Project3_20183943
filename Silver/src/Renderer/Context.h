#pragma once

struct GLFWwindow;

namespace Silver {

	class Context
	{
	public:
		Context(GLFWwindow* window);
		~Context() = default;

		void Init();
		void SwapBuffers();

	private:
		GLFWwindow* m_WindowHandle;
	};

}