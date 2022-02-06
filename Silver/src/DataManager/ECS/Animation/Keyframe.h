#pragma once
#include <string>
#include <glm/gtc/type_ptr.hpp>

namespace Silver {

	class KeyFrame
	{
	public:
		KeyFrame(float keyTime);

		void SetTransform(const float mat[16]);
		float GetKeyTime() { return m_KeyTime; }
		glm::mat4 GetJointTransforms() { return m_JointLocalTransform; }

	private:
		float m_KeyTime;
		glm::mat4 m_JointLocalTransform;
	};

}