#pragma once

#include <glm/glm.hpp>

namespace Silver {

	class Camera
	{
	public:
		virtual ~Camera() = default;

		void SetPosition(const glm::vec3& position);
		void SetXRotation(float angle);
		void SetYRotation(float angle);
		void SetZRotation(float angle);

		const glm::vec3& GetPosition() const { return m_Position; }
		float GetXRotation() const { return glm::degrees(m_XRotation); }
		float GetYRotation() const { return glm::degrees(m_YRotation); }
		float GetZRotation() const { return glm::degrees(m_ZRotation); }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	protected:
		virtual void UpdateViewMatrix();

	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_XRotation = 0.0f;
		float m_YRotation = 0.0f;
		float m_ZRotation = 0.0f;
	};

}