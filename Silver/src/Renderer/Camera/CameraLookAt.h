#pragma once
#include "Camera.h"

namespace Silver {

	class CameraLookAt : public Camera
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
		void RotationLeft(float angle);
		void RotationRight(float angle);
		void RotationUp(float angle);
		void RotationDown(float angle);

		const glm::vec3& GetTargetPos() const { return m_Target; }
		const glm::vec3& GetCameraPos() const { return m_Position; }

	private:
		glm::mat4 GetWorldMatrix();
		glm::vec3 GetXAxis();
		glm::vec3 GetYAxis();
		glm::vec3 GetZAxis();
		void UpdateViewMatrix();

	private:	
		glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };
		glm::vec3 m_Target = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3  m_XAxis, m_YAxis, m_ZAxis;
		float m_RotationLock = 0.0f;
	};

}