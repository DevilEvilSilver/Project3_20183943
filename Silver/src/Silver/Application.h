#pragma once
#include "Core.h"
#include "Singleton.h"
#include "Silver/Events/ApplicationEvent.h"
#include "Window.h"
#include "Silver/LayerStack.h"
#include "Silver/ImGui/ImGuiLayer.h"

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

		Window& GetWindow() { return *m_Window; }

		static Application& GetInstance() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& event);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		// SINGLETON 
		static Application* s_Instance; // Haven't delete anywhere
	};

	//To be define in applications
	Application* CreateApplication();

}
