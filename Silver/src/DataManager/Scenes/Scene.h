#pragma once

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

		void OnUpdate(float deltaTime);
		void OnViewportResize(float width, float height);

	public:
		entt::registry m_Registry;
		float m_ViewportWidth = 0.0f, m_ViewportHeight = 0.0f;
	};

}