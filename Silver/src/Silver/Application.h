#pragma once
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "Silver/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Camera/OrthographicCamera.h"

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
		std::shared_ptr<VertexArray> m_TriangleVA;
		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Shader> m_TriangleShader;
		std::shared_ptr<Shader> m_SquareShader;

		OrthographicCamera m_Camera;
	
	private:
		// SINGLETON 
		static Application* s_Instance; // !!! Haven't delete anywhere
	};

	//To be define in applications
	Application* CreateApplication();

}
