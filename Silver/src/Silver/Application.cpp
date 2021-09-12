#include "pch.h"
#include "Application.h"

#include "Silver/Log.h"
#include "Silver/Events/ApplicationEvent.h"

namespace Silver {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		SV_TRACE(e.GetName());

		while (true);
	}

}