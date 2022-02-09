#include "pch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Silver {

	Camera::Camera(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		SetOrthographic(left, right, bottom, top, zNear, zFar);
	}

	Camera::Camera(float fov, float aspect, float zNear, float zFar)
	{
		SetPerspective(fov, aspect, zNear, zFar);
	}

	void Camera::ResizeAspectRatio(float width, float height)
	{
		if (height != 0)
		{
			m_AspectRatio = width / height;
			ResetProjectionMatrix();
		}
	}

	void Camera::SetProjectionType(ProjectionType type)
	{
		m_ProjectionType = type;
		ResetProjectionMatrix();
	}

	void Camera::SetOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
		m_OrthoSize = top - bottom; m_AspectRatio = (right - left) / (top - bottom);
		m_OrthographicNear = zNear; m_OrthographicFar = zFar;
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::SetPerspective(float fov, float aspect, float zNear, float zFar)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = fov; m_AspectRatio = aspect; m_PerspectiveNear = zNear; m_PerspectiveFar = zFar;
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::ResetProjectionMatrix()
	{
		if (m_ProjectionType == ProjectionType::Orthographic)
			m_ProjectionMatrix = glm::ortho(-m_OrthoSize * m_AspectRatio * 0.5f, m_OrthoSize * m_AspectRatio * 0.5f, 
				-m_OrthoSize * 0.5f, m_OrthoSize * 0.5f, m_OrthographicNear, m_OrthographicFar);
		else if (m_ProjectionType == ProjectionType::Perspective)
			m_ProjectionMatrix = glm::perspective(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}