#pragma once
#include "CameraLookAt.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace Silver {

	class EditorCameraController
	{
	public:
		EditorCameraController(float aspect);

		void OnUpdate(float deltaTime);
		void OnEvent(Event& e);
		void OnResize(float width, float height);

		const CameraLookAt& GetCamera() { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		CameraLookAt m_Camera;
		float m_CameraRotationSpeed = 90.0f;
		float m_CameraZoomSpeed = 0.5f;
	};

}