#include "pch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Silver {

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		UpdateViewMatrix();
	}

	void Camera::SetXRotation(float angle)
	{
		m_XRotation = glm::radians(angle);
		UpdateViewMatrix();
	}

	void Camera::SetYRotation(float angle)
	{
		m_YRotation = glm::radians(angle);
		UpdateViewMatrix();
	}

	void Camera::SetZRotation(float angle)
	{
		m_ZRotation = glm::radians(angle);
		UpdateViewMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), m_YRotation, glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), m_XRotation, glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), m_ZRotation, glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}