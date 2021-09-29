#pragma once

#include "Silver/Core.h"
#include "Silver/Events/Event.h"

namespace Silver {

	class SILVER_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_Name; }

	protected:
		std::string m_Name;
	};

}