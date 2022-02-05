#pragma once

#include <glm/glm.hpp>

namespace Silver {

	class Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		Camera(float left, float right, float bottom, float top, float zNear, float zFar);
		Camera(float fov, float aspect, float zNear, float zFar);
		virtual ~Camera() = default;

		virtual void ResizeAspectRatio(float width, float height);

		virtual void SetOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);
		virtual void SetPerspective(float fov, float aspect, float zNear, float zFar);

		virtual const ProjectionType& GetProjectionType() const { return m_ProjectionType; }
		virtual const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	protected:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix{1.0f};
		glm::mat4 m_ViewProjectionMatrix;

	private:
		float m_AspectRatio;
		float m_PerspectiveFOV, m_PerspectiveNear, m_PerspectiveFar;
		float m_OrthoSize, m_OrthographicNear, m_OrthographicFar;
	};

}