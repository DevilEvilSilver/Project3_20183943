#pragma once
#include "DataManager/Scenes/Scene.h"
#include "DataManager/ECS/Entity.h"
#include <memory>

namespace Silver {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);
		~SceneHierarchyPanel() = default;

		void SetContext(const std::shared_ptr<Scene>& scene);
		void SetSelectedEntity(const Entity& entity);
		Entity GetSelectedEntity() const { return m_SelectionContext; }

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity& entity);
		void DrawComponents(Entity& entity);

	private:
		std::shared_ptr<Scene> m_Context;
		Entity m_SelectionContext {entt::null, m_Context.get()};
		Entity m_CopiededContext = { entt::null, m_Context.get() };
	};

}