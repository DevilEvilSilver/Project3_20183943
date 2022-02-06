#include "pch.h"
#include "Skeleton.h"

namespace Silver {

	Joint::Joint(unsigned int index, const std::string& nameID, glm::mat4 bindLocalTransform)
		:m_Index(index), m_NameID(nameID), m_LocalBindTransform(bindLocalTransform)
	{
	}

	void Joint::addChild(std::shared_ptr<Joint> child)
	{
		m_Children.push_back(child);
	}

	void Joint::calInverseBindTransforms(glm::mat4& parentBindTransform)
	{
		glm::mat4 bindTransform = parentBindTransform * m_LocalBindTransform;
		m_InverseBindTransform = glm::inverse(bindTransform);
		for (auto& child : m_Children) {
			child->calInverseBindTransforms(bindTransform);
		}
	}

	void Joint::SetAnimatedTransform(const glm::mat4& transform)
	{
		m_AnimatedTransform = transform;
	}

	Skeleton::Skeleton(unsigned int jointCount, const std::shared_ptr<Joint>& headJoint)
		:m_JointCount(jointCount), m_HeadJoint(headJoint)
	{
		m_HeadJoint->calInverseBindTransforms(glm::mat4(1.0f));
	}

	const std::vector<glm::mat4>& Skeleton::GetJointTransforms()
	{
		m_JointTransforms.clear();
		JointTransformRecursion(m_HeadJoint);
		//std::sort(m_JointTransforms.begin(), m_JointTransforms.end(), [](std::shared_ptr<Joint> a, std::shared_ptr<Joint> b)
		//{
		//	return a->GetIndex() < b->GetIndex();
		//});
		return m_JointTransforms;
	}

	void Skeleton::JointTransformRecursion(std::shared_ptr<Joint> joint)
	{
		m_JointTransforms.push_back(joint->GetAnimatedTransform());
		for (auto& child : joint->GetChilds())
		{
			JointTransformRecursion(child);
		}
	}

}