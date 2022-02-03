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

		const CameraLookAt& GetCamera() { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		CameraLookAt m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 3.0f };
		float m_CameraXRotation = 0.0f;
		float m_CameraYRotation = 0.0f;
		float m_CameraZRotation = 0.0f;

		float m_CameraRotationSpeed = 90.0f;
		float m_CameraZoomSpeed = 0.5f;
	};

}