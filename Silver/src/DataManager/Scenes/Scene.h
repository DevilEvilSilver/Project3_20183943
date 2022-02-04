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

	public:
		entt::registry m_Registry;
	};

}