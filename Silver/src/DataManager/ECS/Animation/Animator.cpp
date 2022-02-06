#include "pch.h"
#include "Animator.h"

namespace Silver{

	Animator::Animator()
	{
	}

	void Animator::SetAnimation(const std::shared_ptr<Animation>& animtion)
	{
		m_CurrAnimation = animtion;
	}

	void Animator::OnUpdate(float deltaTime)
	{
		if (!HasAnimation()) {
			return;
		}
		IncreaseCurrentTime(deltaTime);
		CalculateCurrentAnimationPose(deltaTime);
	}

	void Animator::ApplyPoseToJoints(std::shared_ptr<Joint>& joint, glm::mat4& parentTransform)
	{
		if (!HasAnimation()) {
			return;
		}
		glm::mat4 currentLocalTransform = m_CurrPose[joint->GetName()];
		glm::mat4 currentTransform = parentTransform * currentLocalTransform;
		for (auto& childJoint : joint->GetChilds()) {
			ApplyPoseToJoints(childJoint, currentTransform);
		}
		currentTransform = currentTransform * joint->GetInverseBindTransform();
		joint->SetAnimatedTransform(currentTransform);
	}

	void Animator::IncreaseCurrentTime(float deltaTime)
	{
		m_CurrTime += deltaTime;
		while (m_CurrTime > m_CurrAnimation->GetDuration()) 
			m_CurrTime -= m_CurrAnimation->GetDuration();
	}

	void Animator::CalculateCurrentAnimationPose(float deltaTime)
	{
		m_CurrPose.clear();
		for (auto& jointAnim : m_CurrAnimation->GetJointAnimations())
		{
			std::shared_ptr<KeyFrame> prev = nullptr, next = nullptr;
			for (auto& keyframe : jointAnim->GetKeyFrames())
			{
				if (m_CurrTime <= keyframe->GetKeyTime())
				{
					next = keyframe;
					break;
				}
				prev = keyframe;
			}
			if (next == nullptr)
				continue;
			if (prev == nullptr)
				prev = jointAnim->GetLastKeyFrame();

			float progression = (m_CurrTime - prev->GetKeyTime()) / (next->GetKeyTime() - prev->GetKeyTime());
			m_CurrPose[jointAnim->GetName()] = Interpolate(prev, next, progression);
		}
	}

	glm::mat4 Animator::Interpolate(std::shared_ptr<KeyFrame> prev, std::shared_ptr<KeyFrame> next, float progression)
	{
		glm::quat firstQuat = glm::quat_cast(prev->GetJointTransforms());
		glm::quat secondQuat = glm::quat_cast(next->GetJointTransforms());
		return glm::mat4_cast(glm::slerp(firstQuat, secondQuat, progression));
	}

}