#pragma once
#include "DataManager/Scenes/Scene.h"

#include "entt.hpp"
#include <memory>

namespace Silver {

	class Entity
	{
	public:
		Entity(entt::entity handle, Scene* scene);

		template<typename T, typename ...Args>
		T& AddComponent(Args&&... args)
		{
			if (HasComponent<T>())
			{
				SV_CORE_ERROR("Entity already has component");
				return GetComponent<T>();
			}
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			if (!HasComponent<T>())
				SV_CORE_ERROR("Entity doesn't have component");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			if (!HasComponent<T>())
			{
				SV_CORE_ERROR("Entity doesn't have component");
				return;
			}	
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		bool operator ==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene;
	};

}