#pragma once
#include "DataManager/Resources/Model/Mesh.h"
#include "DataManager/Resources/Model/Skeleton.h"

#include <string>

namespace Silver {

	class ColladaLoader
	{
	public:
		static void LoadAnimatedModel(const std::string& filepath, 
			std::shared_ptr<Skeleton>& joints, std::vector<std::shared_ptr<Mesh>>& meshes);
		static void LoadStaticModel(const std::string& filepath, std::vector<std::shared_ptr<Mesh>>& meshes);
	};

}