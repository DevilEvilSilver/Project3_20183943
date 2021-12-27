#pragma once
#include "DataManager/Resources/Model/Skeleton.h"
#include "SkinLoader.h"

#include <memory>
#include <string>
#include <tinyxml2.h>

namespace Silver {

	class SkeletonLoader
	{
	public:
		SkeletonLoader() = default;
		~SkeletonLoader() = default;
		void ExtractBoneData(tinyxml2::XMLElement* node,
			const std::vector<std::string>& jointOrder, std::shared_ptr<Skeleton>& joints);

	private:
		std::shared_ptr<Joint> LoadJointData(tinyxml2::XMLElement* node, const std::vector<std::string>& jointOrder, bool isRoot);
		std::shared_ptr<Joint> ExtractJointData(tinyxml2::XMLElement* node, const std::vector<std::string>& jointOrder, bool isRoot);

	private:
		unsigned int m_JointCount = 0;
	};

}