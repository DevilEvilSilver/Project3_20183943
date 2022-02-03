#pragma once

#include "entt.hpp"

namespace Silver {

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

	private:
		entt::registry m_Registry;
	};

}