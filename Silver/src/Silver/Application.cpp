#include "pch.h"
#include "Application.h"

#include "Silver/Log.h"
#include "Silver/Events/ApplicationEvent.h"

namespace Silver {

#define BIND_EVENT_FN(x)  std::bind(&x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		SV_CORE_INFO("{0}", e);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Window->OnUpdate();
		}
	}

}