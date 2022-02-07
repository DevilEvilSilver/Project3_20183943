#include "pch.h"
#include "SkeletonLoader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Silver {

	void SkeletonLoader::ExtractBoneData(tinyxml2::XMLElement* node,
		const std::vector<std::string>& jointOrder, std::shared_ptr<Skeleton>& joints)
	{
		tinyxml2::XMLElement* armatureNode = node
			->FirstChildElement("visual_scene")
			->FirstChildElement("node");
		while (armatureNode != NULL)
		{
			if (std::string(armatureNode->Attribute("id")) == "Armature")
				break;
			armatureNode = armatureNode->NextSiblingElement("node");
		}

		std::shared_ptr<Joint> headJoint = LoadJointData(armatureNode->FirstChildElement("node"), jointOrder, true);
		joints = std::make_shared<Skeleton>(m_JointCount, headJoint);
	}

	std::shared_ptr<Joint> SkeletonLoader::LoadJointData(tinyxml2::XMLElement* node, const std::vector<std::string>& jointOrder, bool isRoot)
	{
		std::shared_ptr<Joint> joint = ExtractJointData(node, jointOrder, isRoot);

		tinyxml2::XMLElement* childNode = node->FirstChildElement("node");
		while (childNode != NULL)
		{
			joint->addChild(LoadJointData(childNode, jointOrder, false));
			childNode = childNode->NextSiblingElement("node");
		}
		return joint;
	}

	std::shared_ptr<Joint> SkeletonLoader::ExtractJointData(tinyxml2::XMLElement* node, const std::vector<std::string>& jointOrder, bool isRoot)
	{
		std::string nameId = node->Attribute("id");
		unsigned int index = std::distance(jointOrder.begin(), std::find(jointOrder.begin(), jointOrder.end(), node->Attribute("sid")));
		float matrixData[16];

		// Get matrix data
		std::string rawData = node->FirstChildElement("matrix")->GetText();
		for (unsigned int i = 0; i < 16; i++)
		{
			matrixData[i] = std::stof(rawData.substr(0, rawData.find(" ")));
			rawData.erase(0, rawData.find(" ") + 1);
		}
		glm::mat4 matrix = glm::make_mat4(matrixData);
		glm::transpose(matrix);
		m_JointCount++;
		return std::make_shared<Joint>(index, nameId, matrix);
	}

}