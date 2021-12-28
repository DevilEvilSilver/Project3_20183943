#pragma once

#include <glm/glm.hpp>

namespace Silver {

	class Camera
	{
	public:
		virtual ~Camera() = default;

		virtual void SetPosition(const glm::vec3& position);
		virtual void SetXRotation(float angle);
		virtual void SetYRotation(float angle);
		virtual void SetZRotation(float angle);

		virtual const glm::vec3& GetPosition() const { return m_Position; }
		virtual float GetXRotation() const { return glm::degrees(m_XRotation); }
		virtual float GetYRotation() const { return glm::degrees(m_YRotation); }
		virtual float GetZRotation() const { return glm::degrees(m_ZRotation); }
		virtual const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		virtual const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	protected:
		virtual void UpdateViewMatrix();

	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };
		float m_XRotation = 0.0f;
		float m_YRotation = 0.0f;
		float m_ZRotation = 0.0f;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		virtual ~OrthographicCamera() = default;
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float aspect, float zNear, float zFar);
		virtual ~PerspectiveCamera() = default;
	};

}