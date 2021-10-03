#pragma once
#include "Core.h"
#include "Singleton.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "Silver/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"

namespace Silver {

	class Application
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
		ImGuiLayer* m_ImGuiLayer; //destroyed in m_LayerStack
		bool m_Running = true;
		LayerStack m_LayerStack;

		// tmp
		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

	private:
		// SINGLETON 
		static Application* s_Instance; // !!! Haven't delete anywhere
	};

	//To be define in applications
	Application* CreateApplication();

}
