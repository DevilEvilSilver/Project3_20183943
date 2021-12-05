#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Silver {

	class Joint
	{
	public:
		Joint(unsigned int index, std::string nameID, glm::mat4 bindLocalTransform);
		~Joint() = default;

		void addChild(std::shared_ptr<Joint> child);

	private:
		unsigned int m_Index;
		std::string m_NameID;
		glm::mat4 m_BindLocalTransform;

		std::vector<std::shared_ptr<Joint>> m_Children;
	};

	class Skeleton
	{
	public:
		Skeleton(unsigned int jointCount, std::shared_ptr<Joint>  headJoint);
		~Skeleton() = default;

	private:
		unsigned int m_JointCount;
		std::shared_ptr<Joint> m_HeadJoint;
	};

}