#pragma once
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "Silver/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Utils/Timer.h"

namespace Silver {

	class Application
	{
	public:
		Application(const std::string& name = "Silver App", unsigned int width = 1280, unsigned int height = 720);
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		Window& GetWindow() { return *m_Window; }

		static Application& GetInstance() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer; //destroyed in m_LayerStack
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		Timer m_Timer;
	
	private:
		// SINGLETON 
		static Application* s_Instance; // !!! Haven't delete anywhere
	};

	//To be define in applications
	Application* CreateApplication();

}
