#include "pch.h"
#include "Animation.h"

namespace Silver {

	JointAnimation::JointAnimation(const std::string& name)
		:m_Name(name)
	{
	}

	JointAnimation::~JointAnimation()
	{
		m_KeyFrames.clear();
	}

	void JointAnimation::SetKeyFrames(const std::vector<std::shared_ptr<KeyFrame>>& keyFrames)
	{
		m_KeyFrames = keyFrames;
	}

	const std::shared_ptr<KeyFrame>& JointAnimation::GetLastKeyFrame()
	{
		return m_KeyFrames[m_KeyFrames.size() - 1];
	}

	Animation::Animation(const std::string& name, const std::string& filepath)
		:m_Name(name), m_FilePath(filepath), m_Duration(0.0f)
	{
	}

	Animation::~Animation()
	{
		m_JointAnimations.clear();
	}

	void Animation::AddJointAnimation(const std::shared_ptr<JointAnimation>& jointAnimation)
	{
		m_JointAnimations.push_back(jointAnimation);
		float keyTime = jointAnimation->GetKeyFrames().back()->GetKeyTime();
		m_Duration = (m_Duration < keyTime) ? keyTime : m_Duration;
	}

}