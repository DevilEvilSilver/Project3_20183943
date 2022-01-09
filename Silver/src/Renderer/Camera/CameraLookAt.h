#pragma once
#include "Camera.h"

namespace Silver {

	class CameraLookAt : public PerspectiveCamera
	{
	public:
		CameraLookAt(float fov, float aspect, float zNear, float zFar);
		~CameraLookAt() = default;

		void SetTargetPos(const glm::vec3& position);
		void SetCameraPos(const glm::vec3& position);
		void MoveForward(float dist);
		void MoveBackward(float dist);
		void MoveLeft(float dist);
		void MoveRight(float dist);
		void MoveUp(float dist);
		void MoveDown(float dist);
		void SetRotation(float xAngle, float yAngle, float zAngle = 0.0f); // Not supporting zAngle currently

		const glm::vec3& GetTargetPos() const { return m_Target; }
		const glm::vec3& GetCameraPos() const { return m_Position; }

	private:
		void SetUpRotation();
		void UpdateViewMatrix() override;

	private:
		glm::vec3 m_Target = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Direction;
	};

}