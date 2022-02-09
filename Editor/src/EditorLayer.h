#pragma once
#include "Silver.h"
#include "Panels/SceneHierarchyPanel.h"

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
		std::shared_ptr<SceneHierarchyPanel> m_SceneHierarchyPanel;

		std::shared_ptr<Scene> m_Scene;
		std::shared_ptr<Entity> m_Entity;
		std::shared_ptr<Entity> m_CameraEntity;

		std::shared_ptr<Silver::Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize{0.0f, 0.0f};
		bool m_ViewportFocused = false, m_ViewportHovered = false;
	};

}