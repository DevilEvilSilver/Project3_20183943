#include "pch.h"
#include "Skeleton.h"

namespace Silver {

	Skeleton::Skeleton(unsigned int jointCount, std::shared_ptr<Joint> headJoint)
		:m_JointCount(jointCount), m_HeadJoint(headJoint)
	{
	}

	Joint::Joint(unsigned int index, std::string nameID, glm::mat4 bindLocalTransform)
		:m_Index(index), m_NameID(nameID), m_BindLocalTransform(bindLocalTransform)
	{
	}

	void Joint::addChild(std::shared_ptr<Joint> child)
	{
		m_Children.push_back(child);
	}

}