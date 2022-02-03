#include "pch.h"
#include "EditorCameraController.h"
#include "Events/Input.h"
#include "Events/EventCode.h"

namespace Silver {

	EditorCameraController::EditorCameraController(float aspect)
		:m_Camera(45.0f, aspect, 0.1f, 1000.0f), m_AspectRatio(aspect)
	{
		m_CameraXRotation = m_Camera.GetXRotation();
		m_CameraYRotation = m_Camera.GetYRotation();
	}

	void EditorCameraController::OnUpdate(float deltaTime)
	{
		//if (Input::IsKeyPressed(KEY_LEFT))
		//	m_Camera.MoveLeft(m_CameraMoveSpeed * deltaTime);
		//else if (Input::IsKeyPressed(KEY_RIGHT))
		//	m_Camera.MoveRight(m_CameraMoveSpeed * deltaTime);
		//if (Input::IsKeyPressed(KEY_DOWN))
		//	m_Camera.MoveBackward(m_CameraMoveSpeed * deltaTime);
		//else if (Input::IsKeyPressed(KEY_UP))
		//	m_Camera.MoveForward(m_CameraMoveSpeed * deltaTime);
		//if (Input::IsKeyPressed(KEY_I))
		//	m_Camera.MoveUp(m_CameraMoveSpeed * deltaTime);
		//else if (Input::IsKeyPressed(KEY_K))
		//	m_Camera.MoveDown(m_CameraMoveSpeed * deltaTime);

		float oldCamYRot = m_CameraXRotation;
		if (Input::IsKeyPressed(KEY_W))
			m_CameraXRotation += m_CameraRotationSpeed * deltaTime;
		else if (Input::IsKeyPressed(KEY_S))
			m_CameraXRotation -= m_CameraRotationSpeed * deltaTime;
		if (Input::IsKeyPressed(KEY_A))
			m_CameraYRotation -= m_CameraRotationSpeed * deltaTime;
		else if (Input::IsKeyPressed(KEY_D))
			m_CameraYRotation += m_CameraRotationSpeed * deltaTime;

		//add rotation lock 
		if (m_CameraXRotation < -80 || m_CameraXRotation > 80)
			m_CameraXRotation = oldCamYRot;
		m_Camera.SetRotation(m_CameraXRotation, m_CameraYRotation);
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