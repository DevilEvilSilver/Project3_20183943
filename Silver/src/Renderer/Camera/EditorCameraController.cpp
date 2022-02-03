#include "pch.h"
#include "EditorCameraController.h"
#include "Events/Input.h"
#include "Events/EventCode.h"

namespace Silver {

	EditorCameraController::EditorCameraController(float aspect)
		:m_Camera(45.0f, aspect, 0.1f, 1000.0f), m_AspectRatio(aspect)
	{
	}

	void EditorCameraController::OnUpdate(float deltaTime)
	{
		if (Input::IsKeyPressed(KEY_W))
			m_Camera.RotationUp(m_CameraRotationSpeed * deltaTime);
		else if (Input::IsKeyPressed(KEY_S))
			m_Camera.RotationDown(m_CameraRotationSpeed * deltaTime);
		if (Input::IsKeyPressed(KEY_A))
			m_Camera.RotationLeft(m_CameraRotationSpeed * deltaTime);
		else if (Input::IsKeyPressed(KEY_D))
			m_Camera.RotationRight(m_CameraRotationSpeed * deltaTime);
	}

	void EditorCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_FN(EditorCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_FN(EditorCameraController::OnWindowResized));
	}

	bool EditorCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Camera.MoveForward(m_CameraZoomSpeed * e.GetYOffset());
		return false;
	}
	bool EditorCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(45.0f, m_AspectRatio, 0.1f, 1000.0f);
		return false;
	}
}