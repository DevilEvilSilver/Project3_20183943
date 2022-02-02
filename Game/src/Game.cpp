#include <Silver.h>

#include "imgui.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TestLayer : public Silver::Layer
{
public:
	TestLayer()
		:Layer("TestLayer"), m_Camera(45.0f, 16.0f/9, 0.1f, 1000.0f), m_CameraPosition(0.0f, 0.0f, 3.0f), m_Position(0.0f), m_SquareColor({ 0.2f, 0.1f, 0.6f })
	{
		m_CameraXRotation = m_Camera.GetXRotation();
		m_CameraYRotation = m_Camera.GetYRotation();

		// Init Square
		{
			float vertices[3 * 4] = {
				-0.7f, -0.7f, 0.0f,
				0.7f, -0.7f, 0.0f,
				0.7f, 0.7f, 0.0f, 
				-0.7f, 0.7f, 0.0f

			};
			std::shared_ptr<Silver::VertexBuffer> squareVB;
			squareVB = std::make_shared<Silver::VertexBuffer>(vertices, sizeof(vertices));
			squareVB->SetLayout({
				{ Silver::DataType::Float3, "a_Position"},
				});
			float normals[2 * 4] = {
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f

			};
			std::shared_ptr<Silver::VertexBuffer> squareNormal;
			squareNormal = std::make_shared<Silver::VertexBuffer>(normals, sizeof(normals));
			squareNormal->SetLayout({
				{ Silver::DataType::Float2, "a_TexCoord"}
				});
			std::vector<std::shared_ptr<Silver::VertexBuffer>> VBList;
			VBList.push_back(squareVB);
			VBList.push_back(squareNormal);

			unsigned int indices[2 * 3] = {
				0, 1, 2,
				0, 2, 3
			};
			std::shared_ptr<Silver::IndexBuffer> squareIB;
			squareIB = std::make_shared<Silver::IndexBuffer>(indices, std::size(indices));

			auto squareMesh = std::make_shared<Silver::Mesh>(VBList, squareIB);

			std::vector<std::shared_ptr<Silver::Mesh>> meshes;
			meshes.push_back(squareMesh);
			m_SquareModel = std::make_shared<Silver::Model>("squareModel", meshes);
			m_ModelLibrary.Add(m_SquareModel);

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
		}
		// Init char
		{
			auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

			m_Texture = std::make_shared<Silver::Texture2D>("assets/textures/char.tga");
			textureShader->Bind();
			textureShader->SubmitUniformInt("u_Texture", 0);
		}

		// Init 3D model
		{
			//m_3DModel = m_ModelLibrary.Load("F:/_Work/_School/Project_3/repository/Game/assets/models/duck.dae");
			m_Cube = m_ModelLibrary.LoadStatic("assets/models/cube.dae");
			m_3DModel = m_ModelLibrary.LoadAnimated("assets/models/originAnimModel.dae");
			m_3DTexture = std::make_shared<Silver::Texture2D>("assets/textures/animTexture.png");
			m_ModelShader = m_ShaderLibrary.Load("assets/shaders/Model.glsl");	
			m_AnimModelShader = m_ShaderLibrary.Load("assets/shaders/Model.glsl");
		}
	}

	void OnAttach() override
	{
		Silver::FramebufferSpec spec;
		spec.Width = 1280; 
		spec.Height = 720;
		m_Framebuffer = std::make_shared<Silver::Framebuffer>(spec);
	}

	void OnUpdate(float deltaTime) override
	{
		//SV_TRACE("Delta Time: {0} ({1} ms)", deltaTime, deltaTime * 1000.f);

		if (Silver::Input::IsKeyPressed(KEY_LEFT))
			m_Camera.MoveLeft(m_CameraMoveSpeed * deltaTime);
		else if (Silver::Input::IsKeyPressed(KEY_RIGHT))
			m_Camera.MoveRight(m_CameraMoveSpeed * deltaTime);
		if (Silver::Input::IsKeyPressed(KEY_DOWN))
			m_Camera.MoveBackward(m_CameraMoveSpeed * deltaTime);
		else if (Silver::Input::IsKeyPressed(KEY_UP))
			m_Camera.MoveForward(m_CameraMoveSpeed * deltaTime);
		if (Silver::Input::IsKeyPressed(KEY_I))
			m_Camera.MoveUp(m_CameraMoveSpeed * deltaTime);
		else if (Silver::Input::IsKeyPressed(KEY_K))
			m_Camera.MoveDown(m_CameraMoveSpeed * deltaTime);

		float oldCamYRot = m_CameraXRotation;
		if (Silver::Input::IsKeyPressed(KEY_W))
			m_CameraXRotation += m_CameraRotationSpeed * deltaTime;
		else if (Silver::Input::IsKeyPressed(KEY_S))
			m_CameraXRotation -= m_CameraRotationSpeed * deltaTime;
		if (Silver::Input::IsKeyPressed(KEY_A))
			m_CameraYRotation -= m_CameraRotationSpeed * deltaTime;
		else if (Silver::Input::IsKeyPressed(KEY_D))
			m_CameraYRotation += m_CameraRotationSpeed * deltaTime;
		//add rotation lock 
		if (m_CameraXRotation < -80 || m_CameraXRotation > 80)
			m_CameraXRotation = oldCamYRot;
		m_Camera.SetRotation(m_CameraXRotation, m_CameraYRotation);

		if (Silver::Input::IsKeyPressed(KEY_J))
			m_Position.x -= m_Speed * deltaTime;
		else if (Silver::Input::IsKeyPressed(KEY_L))
			m_Position.x += m_Speed * deltaTime;
		
		
		glm::mat4 triangleWorldMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		glm::mat4 tileScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_Framebuffer->Bind();
		Silver::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Silver::RenderCommand::Clear();

		Silver::Renderer::BeginScene(m_Camera);

		//m_SquareShader->Bind();
		//m_SquareShader->SubmitUniformFloat3("u_Color", m_SquareColor);
		//for (unsigned int x = 0; x < 20; x++)
		//{
		//	for (unsigned int y = 0; y < 20; y++)
		//	{
		//		glm::vec3 pos(x * 0.15f, y * 0.15f, 0.0f);
		//		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * tileScale;
		//		Silver::Renderer::Submit(m_SquareShader, m_SquareModel, transform);
		//	}
		//}

		//auto textureShader = m_ShaderLibrary.Get("Texture");
		//m_Texture->Bind();
		//Silver::Renderer::Submit(textureShader, m_SquareModel, triangleWorldMatrix);

		m_ModelShader->Bind();
		m_3DTexture->Bind();
		m_ModelShader->SubmitUniformInt("u_Texture", 0);
		Silver::Renderer::Submit(m_AnimModelShader, m_3DModel, glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)));

		Silver::Renderer::EndScene();
		m_Framebuffer->Unbind();
	}

	void OnImGuiRender() override
	{
		ImTextureID textureID = (void*)m_Framebuffer->GetColorAttachmentRendererID();

		//ImGui::Begin("Setting");
		//ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		//ImGui::Image(textureID, ImVec2(1280, 720));
		//ImGui::End();

		ImGui::Begin("View port");
		ImGui::Image(textureID, ImVec2(1280, 720), ImVec2{ 0,1 }, ImVec2{ 1,0 }); // extra param for ImGui weird behavior with uv
		ImGui::End();
	}

	void OnEvent(Silver::Event& event) override
	{

	}

private:
	// tmp
	Silver::ShaderLibrary m_ShaderLibrary;
	Silver::ModelLibrary m_ModelLibrary;
	std::shared_ptr<Silver::Model> m_SquareModel, m_Cube;
	std::shared_ptr <Silver::AnimatedModel> m_3DModel;
	std::shared_ptr<Silver::Shader> m_SquareShader, m_ModelShader, m_AnimModelShader;
	std::shared_ptr<Silver::Texture2D> m_Texture, m_3DTexture;
	std::shared_ptr<Silver::Framebuffer> m_Framebuffer;

	Silver::CameraLookAt m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 2.0f;
	float m_CameraXRotation = 0.0f;
	float m_CameraYRotation = 0.0f;
	float m_CameraZRotation = 0.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquareColor;
	glm::vec3 m_Position;
	float m_Speed = 2.0f;
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