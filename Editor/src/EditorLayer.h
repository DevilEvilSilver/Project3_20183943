#pragma once
#include "Silver.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "EditorCamera.h"
#include "Events/KeyEvent.h"

#include <filesystem>

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
		void UIToolBar();

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
		void NewScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();
		void OnScenePlay();
		void OnSceneStop();

	private:
		std::shared_ptr<Scene> m_Scene;
		std::shared_ptr<EditorCamera> m_EditorCamrera;
		int m_GizmoType = -1;
		Entity m_HoveredEntity = { entt::null, m_Scene.get() };

		std::shared_ptr<Silver::Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize{0.0f, 0.0f};
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;

		// Panels
		std::shared_ptr<SceneHierarchyPanel> m_SceneHierarchyPanel;
		std::shared_ptr<ContentBrowserPanel> m_ContentBrowserPanel;

		// Editor resources
		std::shared_ptr<Texture2D> m_IconPlay, m_IconStop;
	};

}