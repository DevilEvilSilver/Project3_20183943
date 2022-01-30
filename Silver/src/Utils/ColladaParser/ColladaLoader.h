#pragma once
#include "DataManager/Resources/Model/Model.h"
#include "DataManager/Resources/Model/Animation.h"

#include <string>
#include <vector>

namespace Silver {

	class ColladaLoader
	{
	public:
		static void LoadAnimatedModel(const std::string& filepath, 
			std::shared_ptr<Skeleton>& joints, std::vector<std::shared_ptr<Mesh>>& meshes);
		static void LoadStaticModel(const std::string& filepath, std::vector<std::shared_ptr<Mesh>>& meshes);
		static std::vector<std::shared_ptr<Animation>> LoadAnimation(const std::string& filepath);
	};

}