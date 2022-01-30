#include "pch.h"
#include "AnimationLoader.h"

namespace Silver {

	AnimationLoader::~AnimationLoader()
	{
		m_AnimList.clear();
	}

	std::vector<std::shared_ptr<Animation>> AnimationLoader::ExtractAnimattionData(tinyxml2::XMLElement* node, const std::string& filepath)
	{
		m_AnimList.clear();
		m_CurrDirectory = filepath;

		tinyxml2::XMLElement* anim = node->FirstChildElement("animation");
		std::shared_ptr<Animation> defaultAnim = std::make_shared<Animation>(DEFAULT_ANIMATION, filepath);
		while (anim != NULL)
		{
			if (anim->FirstChildElement("animation") == NULL)
			{
				defaultAnim->AddJointAnimation(ExtractJointAnimData(anim));
			}
			else {
				AnimNodeRecursive(anim);
			}
			anim = anim->NextSiblingElement("animation");
		}
		m_AnimList.push_back(defaultAnim);

		return m_AnimList;
	}

	void AnimationLoader::AnimNodeRecursive(tinyxml2::XMLElement* node)
	{
		if (node->FirstChildElement("animation")->FirstChildElement("animation") == NULL)
		{
			std::string nameId = node->Attribute("id");
			std::shared_ptr<Animation> anim = std::make_shared<Animation>(nameId, m_CurrDirectory);

			tinyxml2::XMLElement* joint = node->FirstChildElement("animation");
			while (joint != NULL)
			{
				anim->AddJointAnimation(ExtractJointAnimData(joint));
				joint = joint->NextSiblingElement("animation");
			}
			m_AnimList.push_back(anim);
		}
		else {
			AnimNodeRecursive(node->FirstChildElement("animation"));
		}

		// Check for next sibling node
		if (node->NextSiblingElement("animation") != NULL)
		{
			AnimNodeRecursive(node->NextSiblingElement("animation"));
		}
	}

	std::shared_ptr<JointAnimation> AnimationLoader::ExtractJointAnimData(tinyxml2::XMLElement * animNode)
	{
		// Get source ID
		std::string inputID, outputID;
		tinyxml2::XMLElement* node = animNode->FirstChildElement("sampler")->FirstChildElement("input");
		while (node != NULL) {
			// Get input ID
			if (std::string(node->Attribute("semantic")) == "INPUT") {
				std::string inputSource = node->Attribute("source");
				inputID = inputSource.substr(1);
				node = node->NextSiblingElement("input");
			}
			// Get output ID
			else if (std::string(node->Attribute("semantic")) == "OUTPUT") {
				std::string outputSource = node->Attribute("source");
				outputID = outputSource.substr(1);
				node = node->NextSiblingElement("input");
			}
			//
			// should get interpolation source ?
			//
			else {
				node = node->NextSiblingElement("input");
			}
		}

		std::string target = animNode->FirstChildElement("channel")->Attribute("target");
		std::string jointId = target.substr(0, target.find("/"));
		std::shared_ptr<JointAnimation> jointAnim = std::make_shared<JointAnimation>(jointId);

		std::vector<std::shared_ptr<KeyFrame>> keyFrameList;

		InitKeyFrames(animNode, inputID, keyFrameList);
		ExtractKeyFramesData(animNode, outputID, keyFrameList);

		jointAnim->SetKeyFrames(keyFrameList);
		return jointAnim;
	}

	void AnimationLoader::InitKeyFrames(tinyxml2::XMLElement* node, const std::string& id, std::vector<std::shared_ptr<KeyFrame>>& list)
	{
		// Iterate through source elements 
		tinyxml2::XMLElement* source = node->FirstChildElement("source");
		while (source != NULL) {
			if (std::string(source->Attribute("id")) == id) {
				// Get timestep data
				std::string strTimeStepData = source->FirstChildElement("float_array")->GetText();
				unsigned int numTimeStepData;
				source->FirstChildElement("float_array")->QueryUnsignedAttribute("count", &numTimeStepData);
				list.reserve(++numTimeStepData);
				for (unsigned int i = 0; i < numTimeStepData; i++)
				{
					list.push_back(std::make_shared<KeyFrame>(std::stof(strTimeStepData.substr(0, strTimeStepData.find(" ")))));
					strTimeStepData.erase(0, strTimeStepData.find(" ") + 1);
				}
				break;
			}
			source = source->NextSiblingElement("source");
		}
	}

	void AnimationLoader::ExtractKeyFramesData(tinyxml2::XMLElement* node, const std::string& id, std::vector<std::shared_ptr<KeyFrame>>& list)
	{
		// Iterate through source elements 
		tinyxml2::XMLElement* source = node->FirstChildElement("source");
		while (source != NULL) {
			if (std::string(source->Attribute("id")) == id) {
				// Get transform data
				std::string strTransformData = source->FirstChildElement("float_array")->GetText();
				unsigned int numTransformData;
				source->FirstChildElement("float_array")->QueryUnsignedAttribute("count", &numTransformData);
				for (unsigned int i = 0; i < numTransformData / 16; i++)
				{
					float mat[16];
					for (unsigned int i = 0; i < 16; i++)
					{
						mat[i] = std::stof(strTransformData.substr(0, strTransformData.find(" ")));
						strTransformData.erase(0, strTransformData.find(" ") + 1);
					}
					list.at(i)->SetTransform(mat);
				}
				break;
			}
			source = source->NextSiblingElement("source");
		}
	}

}