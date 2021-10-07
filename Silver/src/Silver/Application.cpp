#include "pch.h"
#include "Application.h"

#include <glad/glad.h>

namespace Silver {

	//tmp
	static unsigned int DataTypeToOpenGLType(DataType type)
	{
		switch (type)
		{
		case DataType::Float:	return GL_FLOAT;
		case DataType::Float2:	return GL_FLOAT;
		case DataType::Float3:	return GL_FLOAT;
		case DataType::Float4:	return GL_FLOAT;
		case DataType::Mat3:	return GL_FLOAT;
		case DataType::Mat4:	return GL_FLOAT;
		case DataType::Int:		return GL_INT;
		case DataType::Int2:	return GL_INT;
		case DataType::Int3:	return GL_INT;
		case DataType::Int4:	return GL_INT;
		case DataType::Bool:	return GL_BOOL;
		}

		SV_CORE_ERROR("Unknown DataType: Can't convert DataType to OpenGLType!");
		return 0;
	}


	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// tmp triangle
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.2f, 0.1f, 0.9f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.4f, 0.7f, 0.2f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.8f, 0.1f, 0.2f, 1.0f
		};
		m_VertexBuffer.reset(new VertexBuffer(vertices, sizeof(vertices)));

		{
			VertexLayout layout = {
				{ DataType::Float3, "a_Position"},
				{ DataType::Float4, "a_Color"}
			};
			m_VertexBuffer->SetLayout(layout);
		}	

		unsigned int index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& attrib : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index, 
				attrib.GetComponentCount(), 
				DataTypeToOpenGLType(attrib.type),
				attrib.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)attrib.offset);
			index++;
		}

		unsigned int indices[1 * 3] = {
			0, 1, 2
		};
		m_IndexBuffer.reset(new IndexBuffer(indices, (unsigned int)std::size(indices)));
		
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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
			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// tmp triangle
			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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