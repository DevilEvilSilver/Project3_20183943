#pragma once
#include "DataManager/ECS/Animation/Animation.h"

#include <tinyxml2.h>

namespace Silver {

	class AnimationLoader
	{
	public:
		AnimationLoader() = default;
		~AnimationLoader();
		std::vector<std::shared_ptr<Animation>> ExtractAnimattionData(tinyxml2::XMLElement * node, const std::string& filepath);

	private:
		void AnimNodeRecursive(tinyxml2::XMLElement* node);
		std::shared_ptr<JointAnimation> ExtractJointAnimData(tinyxml2::XMLElement* animNode);
		void InitKeyFrames(tinyxml2::XMLElement* node, const std::string& id, std::vector<std::shared_ptr<KeyFrame>>& list);
		void ExtractKeyFramesData(tinyxml2::XMLElement* node, const std::string& id, std::vector<std::shared_ptr<KeyFrame>>& list);

	private:
		std::string m_CurrDirectory;
		std::vector<std::shared_ptr<Animation>> m_AnimList;
	};

}