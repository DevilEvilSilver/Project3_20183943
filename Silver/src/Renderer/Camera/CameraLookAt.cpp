#include "pch.h"
#include "CameraLookAt.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace Silver {

	CameraLookAt::CameraLookAt(float fov, float aspect, float zNear, float zFar)
		: PerspectiveCamera(fov, aspect, zNear, zFar)
	{
		m_Direction = m_Target - m_Position;
		UpdateViewMatrix();
		SetUpRotation();
	}

	void CameraLookAt::SetTargetPos(const glm::vec3& position)
	{
		if (m_Target != position)
		{
			m_Target = position;
			m_Direction = m_Target - m_Position;
			UpdateViewMatrix();
		}
	}

	void CameraLookAt::SetCameraPos(const glm::vec3& position)
	{
		if (m_Position != position)
		{
			m_Position = position;
			m_Direction = m_Target - m_Position;
			UpdateViewMatrix();
		}
	}

	void CameraLookAt::MoveForward(float dist)
	{
		glm::vec3 delta = glm::normalize(m_Direction) * dist;
		m_Position += delta;
		m_Target += delta;
		UpdateViewMatrix();
	}

	void CameraLookAt::MoveBackward(float dist)
	{
		glm::vec3 delta = glm::normalize(m_Direction) * -dist;
		m_Position += delta;
		m_Target += delta;
		UpdateViewMatrix();
	}

	void CameraLookAt::MoveLeft(float dist)
	{
		glm::vec3 delta = glm::normalize(glm::cross(m_Up, m_Direction)) * dist;
		m_Position += delta;
		m_Target += delta;
		UpdateViewMatrix();
	}

	void CameraLookAt::MoveRight(float dist)
	{
		glm::vec3 delta = glm::normalize(glm::cross(m_Up, m_Direction)) * -dist;
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

	void CameraLookAt::SetRotation(float xAngle, float yAngle, float zAngle)
	{
		m_Direction.x = cos(glm::radians(yAngle)) * cos(glm::radians(xAngle));
		m_Direction.y = sin(glm::radians(xAngle));
		m_Direction.z = sin(glm::radians(yAngle)) * cos(glm::radians(xAngle));
		glm::normalize(m_Direction);
		UpdateViewMatrix();
	}

	void CameraLookAt::SetUpRotation()
	{
		// HAVEN"T CHECK CORRECTION FOR X_ROTATION & Z_ROTATION
		m_XRotation = glm::orientedAngle(
			glm::vec3(0.0f, m_Direction.y, m_Direction.z), 
			glm::vec3(0.0f, 0.0f, -1.0f), 
			glm::vec3(-1.0f, 0.0f, 0.0f));
		m_YRotation = glm::orientedAngle(
			glm::vec3(m_Direction.x, 0.0f, m_Direction.z), 
			glm::vec3(-1.0f, 0.0f, 0.0f), 
			glm::vec3(0.0f, -1.0f, 0.0f));;
		m_ZRotation = glm::orientedAngle(
			glm::vec3(m_Direction.x, m_Direction.y, 0.0f), 
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, -1.0f));
	}

	void CameraLookAt::UpdateViewMatrix()
	{
		glm::vec3 camRight = glm::normalize(glm::cross(m_Up, -m_Direction));
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, glm::cross(-m_Direction, camRight));
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}