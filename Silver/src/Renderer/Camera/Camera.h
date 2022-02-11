#pragma once

#include <glm/glm.hpp>

namespace Silver {

	class Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		Camera() { ResetProjectionMatrix(); };
		Camera(float left, float right, float bottom, float top, float zNear, float zFar);
		Camera(float fov, float aspect, float zNear, float zFar);
		virtual ~Camera() = default;

		virtual void ResizeAspectRatio(float width, float height);

		virtual void SetProjectionType(ProjectionType type);
		virtual void SetOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);
		virtual void SetPerspective(float fov, float aspect, float zNear, float zFar);

		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; ResetProjectionMatrix(); }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; ResetProjectionMatrix(); }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; ResetProjectionMatrix(); }

		float GetOrthographicSize() const { return m_OrthoSize; }
		void SetOrthographicSize(float size) { m_OrthoSize = size; ResetProjectionMatrix(); }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; ResetProjectionMatrix(); }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; ResetProjectionMatrix(); }

		virtual const ProjectionType& GetProjectionType() const { return m_ProjectionType; }
		virtual const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void ResetProjectionMatrix();

	protected:
		ProjectionType m_ProjectionType = ProjectionType::Perspective;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix{1.0f};
		glm::mat4 m_ViewProjectionMatrix;

	private:
		float m_AspectRatio = 16.0f / 9.0f;
		float m_PerspectiveFOV = 45.0f, m_PerspectiveNear = 0.1f, m_PerspectiveFar = 1000.0f;
		float m_OrthoSize = 720, m_OrthographicNear = -1, m_OrthographicFar = 1;
	};

}