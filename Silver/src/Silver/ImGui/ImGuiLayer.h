#pragma once

#include "Silver/Layer.h"
#include "Silver/Events/MouseEvent.h"
#include "Silver/Events/KeyEvent.h"
#include "Silver/Events/ApplicationEvent.h"

namespace Silver {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};

}