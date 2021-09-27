#pragma once
#include "Core.h"
#include "Silver/Events/ApplicationEvent.h"
#include "Window.h"
#include "Silver/LayerStack.h"

namespace Silver {

	class SILVER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& event);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	//To be define in applications
	Application* CreateApplication();

}
