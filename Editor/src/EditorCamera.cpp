#include "EditorCamera.h"
#include "Events/Input.h"
#include "Events/EventCode.h"

namespace Silver {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		:m_Camera(fov, aspectRatio, nearClip, farClip)
	{
	}

	void EditorCamera::OnUpdate(float deltaTime)
	{
		if (Input::IsKeyPressed(KEY_LEFT_ALT))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
				m_Camera.MousePan(delta);
			else if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				m_Camera.MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
				m_Camera.MouseZoom(delta.y);
		}

		m_Camera.UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		m_Camera.MouseZoom(delta);
		m_Camera.UpdateView();
		return false;
	}

}