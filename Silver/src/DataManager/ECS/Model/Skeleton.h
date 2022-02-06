#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Silver {

	class Joint
	{
	public:
		Joint(unsigned int index, const std::string& nameID, glm::mat4 bindLocalTransform);
		~Joint() = default;

		void addChild(std::shared_ptr<Joint> child);
		void calInverseBindTransforms(glm::mat4& parentBindTransform);
		void SetAnimatedTransform(const glm::mat4& transform);

		unsigned int GetIndex() { return m_Index; }
		const std::string& GetName() { return m_NameID; }
		std::vector<std::shared_ptr<Joint>>& GetChilds() { return m_Children; }
		const glm::mat4& GetInverseBindTransform() { return m_InverseBindTransform; }
		const glm::mat4& GetAnimatedTransform() { return m_AnimatedTransform; }

	private:
		unsigned int m_Index = 0;
		std::string m_NameID;
		glm::mat4 m_AnimatedTransform{ 1.0f };
		glm::mat4 m_LocalBindTransform, m_InverseBindTransform;

		std::vector<std::shared_ptr<Joint>> m_Children;
	};

	class Skeleton
	{
	public:
		Skeleton(unsigned int jointCount, const std::shared_ptr<Joint>& headJoint);
		~Skeleton() = default;

		std::shared_ptr<Joint>& GetHeadJoint() { return m_HeadJoint; }
		const std::vector<glm::mat4>& GetJointTransforms();

	private:
		void JointTransformRecursion(std::shared_ptr<Joint> joint);

	private:
		unsigned int m_JointCount;
		std::shared_ptr<Joint> m_HeadJoint;
		std::vector<glm::mat4> m_JointTransforms;
	};

}