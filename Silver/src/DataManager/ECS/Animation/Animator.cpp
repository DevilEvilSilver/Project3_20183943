#include "pch.h"
#include "Animator.h"

namespace Silver{

	Animator::Animator()
	{
	}

	void Animator::BindAnimation(const std::shared_ptr<Animation>& animtion)
	{
		m_CurrAnimation = animtion;
	}

	void Animator::UnbindAnimation()
	{
		m_CurrAnimation = nullptr;
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
		glm::mat4 currentLocalTransform = GetCurrLocalTransform(joint->GetName());
		glm::mat4 currentTransform = currentLocalTransform * parentTransform;
		for (auto& childJoint : joint->GetChilds()) {
			ApplyPoseToJoints(childJoint, currentTransform);
		}
		joint->SetAnimatedTransform(joint->GetInverseBindTransform() * currentTransform);
	}

	glm::mat4 Animator::GetCurrLocalTransform(const std::string& name)
	{
		if (m_CurrPose.find(name) != m_CurrPose.end())
		{
			return m_CurrPose[name];
		}
		return glm::mat4(1.0f);
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
			m_CurrPose[jointAnim->GetName()] = glm::transpose(Interpolate(prev, next, progression));
		}
	}

	glm::mat4 Animator::Interpolate(std::shared_ptr<KeyFrame> prev, std::shared_ptr<KeyFrame> next, float progression)
	{
		glm::vec3 firstPos = prev->GetJointTransforms().Position;
		glm::vec3 secondPos = next->GetJointTransforms().Position;
		glm::quat firstQuat = prev->GetJointTransforms().Rotatation;
		glm::quat secondQuat = next->GetJointTransforms().Rotatation;
		return glm::translate(glm::mat4(1), glm::mix(firstPos, secondPos, progression)) *
			glm::mat4_cast(glm::slerp(firstQuat, secondQuat, progression));
	}

}