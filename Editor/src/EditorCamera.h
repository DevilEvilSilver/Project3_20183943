#pragma once
#include "Renderer/Camera/CameraOrbit.h"

#include <glm/glm.hpp>

namespace Silver {

	// from The Cherno
	class EditorCamera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(float deltaTime);
		void OnEvent(Event& e);

		inline void SetViewportSize(float width, float height) { m_Camera.ResizeAspectRatio(width, height); }

		const glm::mat4& GetViewProjectionMatrix() { return m_Camera.GetViewProjectionMatrix(); }
		const glm::mat4& GetProjectionMatrix() { return m_Camera.GetProjectionMatrix(); }
		const glm::mat4& GetViewMatrix() { return m_Camera.GetViewMatrix(); }

	private:
		bool OnMouseScroll(MouseScrolledEvent& e);

	private:
		CameraOrbit m_Camera;
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };
	};

}