#include "pch.h"
#include "Keyframe.h"

namespace Silver {

	KeyFrame::KeyFrame(float keyTime)
		:m_KeyTime(keyTime)
	{
	}

	void KeyFrame::SetTransform(const float mat[16])
	{
		m_JointLocalTransform = glm::make_mat4(mat);
	}
}