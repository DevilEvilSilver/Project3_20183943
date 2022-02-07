#include "pch.h"
#include "Keyframe.h"

namespace Silver {

	KeyFrame::KeyFrame(float keyTime)
		:m_KeyTime(keyTime)
	{
	}

	void KeyFrame::SetTransform(const float mat[16])
	{
		glm::mat4 transform = glm::transpose(glm::make_mat4(mat));
		m_JointLocalTransform.Position = transform[3];
		m_JointLocalTransform.Rotatation = glm::quat_cast(transform);
	}
}