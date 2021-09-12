#include "pch.h"
#include "Application.h"

#include "Silver/Log.h"
#include "Silver/Events/ApplicationEvent.h"

namespace Silver {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		SV_TRACE(e.GetName());

		while (m_Running)
		{
			m_Window->Update();
		}
	}

}