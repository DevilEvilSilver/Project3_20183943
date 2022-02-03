#pragma once
#include <Silver.h>

namespace Silver {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		// tmp
		std::shared_ptr <Silver::AnimatedModel> m_3DModel;
		std::shared_ptr<Silver::Shader>  m_AnimModelShader;
		std::shared_ptr<Silver::Texture2D> m_3DTexture;
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

		glm::vec2 m_ViewPortSize;
	};

}