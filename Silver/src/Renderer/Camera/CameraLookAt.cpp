#include "pch.h"
#include "CameraLookAt.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Silver {

	CameraLookAt::CameraLookAt(float fov, float aspect, float zNear, float zFar)
		: Camera(fov, aspect, zNear, zFar)
	{
		m_ZAxis = GetZAxis(); m_XAxis = GetXAxis(); m_YAxis = GetYAxis();
		UpdateViewMatrix();
	}

	void CameraLookAt::SetTargetPos(const glm::vec3& position)
	{
		if (m_Target != position)
		{
			m_Target = position;
			m_ZAxis = GetZAxis(); m_XAxis = GetXAxis(); m_YAxis = GetYAxis();
			UpdateViewMatrix();
		}
	}

	void CameraLookAt::SetCameraPos(const glm::vec3& position)
	{
		if (m_Position != position)
		{
			m_Position = position;
			m_ZAxis = GetZAxis(); m_XAxis = GetXAxis(); m_YAxis = GetYAxis();
			UpdateViewMatrix();
		}
	}

	void CameraLookAt::MoveForward(float dist)
	{
		glm::vec3 delta = glm::normalize(-m_ZAxis) * dist;
		m_Position += delta;
		m_Target += delta;
		UpdateViewMatrix();
	}

	void CameraLookAt::MoveBackward(float dist)
	{
		glm::vec3 delta = glm::normalize(-m_ZAxis) * -dist;
		m_Position += delta;
		m_Target += delta;
		UpdateViewMatrix();
	}

	void CameraLookAt::MoveLeft(float dist)
	{
		glm::vec3 delta = glm::normalize(glm::cross(m_Up, -m_ZAxis)) * dist;
		m_Position += delta;
		m_Target += delta;
		UpdateViewMatrix();
	}

	void CameraLookAt::MoveRight(float dist)
	{
		glm::vec3 delta = glm::normalize(glm::cross(m_Up, -m_ZAxis)) * -dist;
		m_Position += delta;
		m_Target += delta;
		UpdateViewMatrix();
	}

	void CameraLookAt::MoveUp(float dist)
	{
		glm::vec3 delta = glm::normalize(m_Up) * dist;
		m_Position += delta;
		m_Target += delta;
		UpdateViewMatrix();
	}

	void CameraLookAt::MoveDown(float dist)
	{
		glm::vec3 delta = glm::normalize(m_Up) * -dist;
		m_Position += delta;
		m_Target += delta;
		UpdateViewMatrix();
	}

	void CameraLookAt::RotationLeft(float angle)
	{
		m_ZAxis = glm::rotate(m_ZAxis, glm::radians(angle), m_YAxis);
		m_Target = m_Position - m_ZAxis;
		m_XAxis = GetXAxis(); m_YAxis = GetYAxis();
		UpdateViewMatrix();
	}

	void CameraLookAt::RotationRight(float angle)
	{
		m_ZAxis = glm::rotate(m_ZAxis, glm::radians(-angle), m_YAxis);
		m_Target = m_Position - m_ZAxis;
		m_XAxis = GetXAxis(); m_YAxis = GetYAxis();
		UpdateViewMatrix();
	}

	void CameraLookAt::RotationUp(float angle)
	{
		if (m_RotationLock + angle < 80)
		{
			m_RotationLock += angle;
			m_ZAxis = glm::rotate(m_ZAxis, glm::radians(angle), m_XAxis);
			m_Target = m_Position - m_ZAxis;
			m_XAxis = GetXAxis(); m_YAxis = GetYAxis();
			UpdateViewMatrix();
		}
	}

	void CameraLookAt::RotationDown(float angle)
	{
		if (m_RotationLock - angle > -80)
		{
			m_RotationLock -= angle;
			m_ZAxis = glm::rotate(m_ZAxis, glm::radians(-angle), m_XAxis);
			m_Target = m_Position - m_ZAxis;
			m_XAxis = GetXAxis(); m_YAxis = GetYAxis();
			UpdateViewMatrix();
		}
	}

	glm::mat4 CameraLookAt::GetWorldMatrix()
	{
		return glm::mat4({
			m_XAxis.x, m_XAxis.y, m_XAxis.z, 0.0f,
			m_YAxis.x, m_YAxis.y, m_YAxis.z, 0.0f,
			m_ZAxis.x, m_ZAxis.y, m_ZAxis.z, 0.0f,
			m_Position.x, m_Position.y, m_Position.z, 1.0f,
		});
	}

	glm::vec3 CameraLookAt::GetXAxis()
	{
		return glm::normalize(glm::cross(m_Up, m_ZAxis));
	}

	glm::vec3 CameraLookAt::GetYAxis()
	{
		return glm::normalize(glm::cross(m_ZAxis, m_XAxis));
	}

	glm::vec3 CameraLookAt::GetZAxis()
	{
		return glm::normalize(m_Position - m_Target);
	}

	void CameraLookAt::UpdateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Target, glm::cross(m_ZAxis, m_XAxis));
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}