#pragma once
#include "Keyframe.h"
#include <vector>
#include <memory>

namespace Silver {
	
	class JointAnimation
	{
	public:
		JointAnimation(const std::string& name);
		~JointAnimation();

		void SetKeyFrames(const std::vector<std::shared_ptr<KeyFrame>>& keyFrames);
		const std::string& GetName() { return m_Name; }
		const std::vector<std::shared_ptr<KeyFrame>>& GetKeyFrames() { return m_KeyFrames; }

	private:
		std::string m_Name;
		std::vector<std::shared_ptr<KeyFrame>> m_KeyFrames;
		// might need interpolation list
	};

	class Animation
	{
	public:
		Animation(const std::string& name, const std::string& filepath);
		~Animation();

		void AddJointAnimation(const std::shared_ptr<JointAnimation>& jointAnimation);
		const std::string& GetName() { return m_Name; }
		const std::string& GetFilePath() { return m_FilePath; }
		float GetDuration() const { return m_Duration; }
		const std::vector<std::shared_ptr<JointAnimation>>& GetJointAnimations() { return m_JointAnimations; }

	private:
		std::string m_Name;
		std::string m_FilePath;
		float m_Duration;
		std::vector<std::shared_ptr<JointAnimation>> m_JointAnimations;
	};

#define DEFAULT_ANIMATION "default_anim"

}