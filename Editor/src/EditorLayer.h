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
		virtual void OnEvent(Event& e) override;

	private:
		// tmp
		std::shared_ptr <Silver::AnimatedModel> m_3DModel;
		std::shared_ptr<Silver::Shader>  m_AnimModelShader;
		std::shared_ptr<Silver::Texture2D> m_3DTexture;
		std::shared_ptr<Silver::Framebuffer> m_Framebuffer;

		Silver::EditorCameraController m_EditorCameraController;

		glm::vec2 m_ViewPortSize;
	};

}