#include "pch.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

namespace Silver {

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Camera(-3.2f, 3.2f, -1.8f, 1.8f)
	{
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// Init Triangle
		{
			m_TriangleVA.reset(new VertexArray());

			float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 0.2f, 0.1f, 0.9f, 1.0f,
				0.5f, -0.5f, 0.0f, 0.4f, 0.7f, 0.2f, 1.0f,
				0.0f, 0.5f, 0.0f, 0.8f, 0.1f, 0.2f, 1.0f
			};
			std::shared_ptr<VertexBuffer> triangleVB;
			triangleVB.reset(new VertexBuffer(vertices, sizeof(vertices)));
			triangleVB->SetLayout({
				{ DataType::Float3, "a_Position"},
				{ DataType::Float4, "a_Color"}
			});
			m_TriangleVA->AddVertexBuffer(triangleVB);

			unsigned int indices[1 * 3] = { 0, 1, 2 };
			std::shared_ptr<IndexBuffer> triangleIB;
			triangleIB.reset(new IndexBuffer(indices, sizeof(indices)));
			m_TriangleVA->SetIndexBuffer(triangleIB);

			std::string vertexSrc = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;
			
				uniform	mat4 u_ViewProjection;	

				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
					v_Position = a_Position;
					v_Color = a_Color;
				}

			)";

			std::string fragmentSrc = R"(
				#version 330 core

				layout(location = 0) out vec4 color;
			
				in vec3 v_Position;
				in vec4 v_Color;

				void main()
				{
					color = vec4(v_Position * 0.5 + 0.5, 1.0);
					color = v_Color;
				}

			)";

			m_TriangleShader.reset(new Shader(vertexSrc, fragmentSrc));
		}
		
		// Init Square
		{
			m_SquareVA.reset(new VertexArray());

			float vertices[3 * 4] = {
				-0.7f, -0.7f, 0.0f,
				0.7f, -0.7f, 0.0f,
				0.7f, 0.7f, 0.0f,
				-0.7f, 0.7f, 0.0f

			};
			std::shared_ptr<VertexBuffer> squareVB;
			squareVB.reset(new VertexBuffer(vertices, sizeof(vertices)));
			squareVB->SetLayout({
				{ DataType::Float3, "a_Position"}
				});
			m_SquareVA->AddVertexBuffer(squareVB);

			unsigned int indices[2 * 3] = {
				0, 1, 2,
				0, 2, 3
			};
			std::shared_ptr<IndexBuffer> squareIB;
			squareIB.reset(new IndexBuffer(indices, (unsigned int)std::size(indices)));
			m_SquareVA->SetIndexBuffer(squareIB);

			std::string vertexSrc2 = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;

				uniform	mat4 u_ViewProjection;
			
				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
					v_Position = a_Position;
				}

			)";

			std::string fragmentSrc2 = R"(
				#version 330 core

				layout(location = 0) out vec4 color;
			
				in vec3 v_Position;
				in vec4 v_Color;

				void main()
				{
					color = vec4(v_Position * 0.5 + 0.5, 1.0);
				}

			)";

			m_SquareShader.reset(new Shader(vertexSrc2, fragmentSrc2));
		}
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (event.m_Handled)
				break;
			(*it)->OnEvent(event);
		}
	}

	void Application::PushLayer(Layer * layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer * layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
			RenderCommand::Clear();

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_SquareShader, m_SquareVA);
			Renderer::Submit(m_TriangleShader, m_TriangleVA);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			// Temporary here, need its own render func/thread
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}