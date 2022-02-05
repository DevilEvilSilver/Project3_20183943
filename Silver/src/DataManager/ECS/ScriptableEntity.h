#pragma once

#include "Entity.h"
#include "Events/Event.h"

namespace Silver {

	class ScriptableEntity
	{
	public:
		ScriptableEntity() = delete;
		ScriptableEntity(const std::shared_ptr<Entity>& entity)
			:m_Entity(entity) {}
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity->GetComponent<T>();
		}

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnEvent(Event& e) {}

	private:
		std::shared_ptr<Entity> m_Entity;
		friend class Scene;
	};

}