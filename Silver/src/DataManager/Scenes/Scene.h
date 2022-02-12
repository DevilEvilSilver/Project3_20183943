#pragma once

#include "Events/Event.h"
#include "entt.hpp"
#include <glm/glm.hpp>

namespace Silver {

	class Entity;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		std::shared_ptr<Entity> CreateEntity(const std::string& name = "");
		void DestroyEntity(Entity& entity);
		void Clear();

		Entity GetPrimaryCameraEntity();

		void OnUpdate(float deltaTime);
		void OnEvent(Event& e);
		void OnViewportResize(float width, float height);

	private:
		entt::registry m_Registry;
		float m_ViewportWidth = 0.0f, m_ViewportHeight = 0.0f;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}