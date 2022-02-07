#pragma once
#include <string>
#include <glm/gtc/type_ptr.hpp>

namespace Silver {

	struct JointTransform
	{
		glm::vec3 Position;
		glm::quat Rotatation;
	};

	class KeyFrame
	{
	public:
		KeyFrame(float keyTime);

		void SetTransform(const float mat[16]);
		float GetKeyTime() { return m_KeyTime; }
		JointTransform GetJointTransforms() { return m_JointLocalTransform; }

	private:
		float m_KeyTime;
		JointTransform m_JointLocalTransform;
	};

}