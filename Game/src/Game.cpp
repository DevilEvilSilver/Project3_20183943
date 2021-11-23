#include <Silver.h>

#include "imgui.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TestLayer : public Silver::Layer
{
public:
	TestLayer()
		:Layer("TestLayer"), m_Camera(45.0f, 16.0f/9, 0.1f, 100.0f), m_CameraPosition(0.0f, 0.0f, 3.0f), m_TrianglePosition(0.0f), m_SquareColor({ 0.2f, 0.1f, 0.6f })
	{
		// Init Triangle
		{
			m_TriangleVA = std::make_shared<Silver::VertexArray>();

			float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 0.2f, 0.1f, 0.9f, 1.0f,
				0.5f, -0.5f, 0.0f, 0.4f, 0.7f, 0.2f, 1.0f,
				0.0f, 0.5f, 0.0f, 0.8f, 0.1f, 0.2f, 1.0f
			};
			std::shared_ptr<Silver::VertexBuffer> triangleVB;
			triangleVB = std::make_shared<Silver::VertexBuffer>(vertices, sizeof(vertices));
			triangleVB->SetLayout({
				{ Silver::DataType::Float3, "a_Position"},
				{ Silver::DataType::Float4, "a_Color"}
				});
			m_TriangleVA->AddVertexBuffer(triangleVB);

			unsigned int indices[1 * 3] = { 0, 1, 2 };
			std::shared_ptr<Silver::IndexBuffer> triangleIB;
			triangleIB = std::make_shared<Silver::IndexBuffer>(indices, std::size(indices));
			m_TriangleVA->SetIndexBuffer(triangleIB);

			std::string vertexSrc = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;
			
				uniform	mat4 u_ViewProjection;	
				uniform	mat4 u_World;

				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					gl_Position = u_ViewProjection * u_World * vec4(a_Position, 1.0);
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

			m_TriangleShader = std::make_shared<Silver::Shader>("TriangleShader", vertexSrc, fragmentSrc);
		}

		// Init Square
		{
			m_SquareVA = std::make_shared<Silver::VertexArray>();

			float vertices[5 * 4] = {
				-0.7f, -0.7f, 0.0f, 0.0f, 0.0f,
				0.7f, -0.7f, 0.0f, 1.0f, 0.0f,
				0.7f, 0.7f, 0.0f, 1.0f, 1.0f,
				-0.7f, 0.7f, 0.0f, 0.0f, 1.0f

			};
			std::shared_ptr<Silver::VertexBuffer> squareVB;
			squareVB = std::make_shared<Silver::VertexBuffer>(vertices, sizeof(vertices));
			squareVB->SetLayout({
				{ Silver::DataType::Float3, "a_Position"},
				{ Silver::DataType::Float2, "a_TexCoord"}
				});
			m_SquareVA->AddVertexBuffer(squareVB);

			unsigned int indices[2 * 3] = {
				0, 1, 2,
				0, 2, 3
			};
			std::shared_ptr<Silver::IndexBuffer> squareIB;
			squareIB = std::make_shared<Silver::IndexBuffer>(indices, std::size(indices));
			m_SquareVA->SetIndexBuffer(squareIB);

			std::string vertexSrc2 = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;

				uniform	mat4 u_ViewProjection;
				uniform	mat4 u_World;

				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					gl_Position = u_ViewProjection * u_World * vec4(a_Position, 1.0);
					v_Position = a_Position;
				}

			)";

			std::string fragmentSrc2 = R"(
				#version 330 core

				layout(location = 0) out vec4 color;
			
				uniform vec3 u_Color;

				in vec3 v_Position;
				in vec4 v_Color;

				void main()
				{
					color = vec4(u_Color, 1.0);
				}

			)";

			m_SquareShader = std::make_shared<Silver::Shader>("SquareShader", vertexSrc2, fragmentSrc2);

			auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

			m_Texture = std::make_shared<Silver::Texture2D>("assets/textures/char.tga");
			textureShader->Bind();
			textureShader->SubmitUniformInt("u_Texture", 0);
		}
	}

	void OnUpdate(float deltaTime) override
	{
		//SV_TRACE("Delta Time: {0} ({1} ms)", deltaTime, deltaTime * 1000.f);

		if (Silver::Input::IsKeyPressed(KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * deltaTime;
		else if (Silver::Input::IsKeyPressed(KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * deltaTime;
		if (Silver::Input::IsKeyPressed(KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * deltaTime;
		else if (Silver::Input::IsKeyPressed(KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * deltaTime;

		if (Silver::Input::IsKeyPressed(KEY_A))
			m_CameraZRotation += m_CameraRotationSpeed * deltaTime;
		else if (Silver::Input::IsKeyPressed(KEY_D))
			m_CameraZRotation -= m_CameraRotationSpeed * deltaTime;

		if (Silver::Input::IsKeyPressed(KEY_J))
			m_TrianglePosition.x -= m_TriangleSpeed * deltaTime;
		else if (Silver::Input::IsKeyPressed(KEY_L))
			m_TrianglePosition.x += m_TriangleSpeed * deltaTime;
		if (Silver::Input::IsKeyPressed(KEY_I))
			m_TrianglePosition.y += m_TriangleSpeed * deltaTime;
		else if (Silver::Input::IsKeyPressed(KEY_K))
			m_TrianglePosition.y -= m_TriangleSpeed * deltaTime;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetZRotation(m_CameraZRotation);
		glm::mat4 triangleWorldMatrix = glm::translate(glm::mat4(1.0f), m_TrianglePosition);
		glm::mat4 tileScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Silver::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Silver::RenderCommand::Clear();

		Silver::Renderer::BeginScene(m_Camera);

		m_SquareShader->Bind();
		m_SquareShader->SubmitUniformFloat3("u_Color", m_SquareColor);

		for (unsigned int x = 0; x < 20; x++)
		{
			for (unsigned int y = 0; y < 20; y++)
			{
				glm::vec3 pos(x * 0.15f, y * 0.15f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * tileScale;
				Silver::Renderer::Submit(m_SquareShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Silver::Renderer::Submit(textureShader, m_SquareVA, triangleWorldMatrix);
		
		//Silver::Renderer::Submit(m_TriangleShader, m_TriangleVA, triangleWorldMatrix);

		Silver::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Silver::Event& event) override
	{

	}

private:
	// tmp
	Silver::ShaderLibrary m_ShaderLibrary;
	std::shared_ptr<Silver::VertexArray> m_TriangleVA;
	std::shared_ptr<Silver::VertexArray> m_SquareVA;
	std::shared_ptr<Silver::Shader> m_TriangleShader;
	std::shared_ptr<Silver::Shader> m_SquareShader;
	std::shared_ptr<Silver::Texture2D> m_Texture;

	Silver::PerspectiveCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 2.0f;
	float m_CameraZRotation = 0.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquareColor;
	glm::vec3 m_TrianglePosition;
	float m_TriangleSpeed = 2.0f;
};

class Game : public Silver::Application
{
public:
	Game()
	{
		PushLayer(new TestLayer());
	}

	~Game()
	{

	}

};

Silver::Application* Silver::CreateApplication()
{
	return new Game;
}

#ifdef SV_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Silver::Log::Init();
	SV_INFO("Start up ! ! !");

	auto app = Silver::CreateApplication();
	app->Run();
	delete app;
}

#else 
	#error Silver only support Windows 
#endif