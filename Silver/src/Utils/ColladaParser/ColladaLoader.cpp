#include "pch.h"
#include "ColladaLoader.h"
#include "SkinLoader.h"
#include "SkeletonLoader.h"
#include "GeometryLoader.h"

#include <tinyxml2.h>

namespace Silver {

	void ColladaLoader::LoadAnimatedModel(const std::string& filepath, 
		std::shared_ptr<Skeleton>& joints, std::vector<std::shared_ptr<Mesh>>& meshes)
	{
		tinyxml2::XMLDocument doc;
		if (doc.LoadFile(filepath.c_str()))
		{
			SV_CORE_ERROR("Load failed");
		}

		const unsigned int MAX_WEIGHT_COUNT = 3; // Currently support (1 vertex in effect of max to 3 joint)
		std::unique_ptr<SkinLoader> skinLoader = 
			std::make_unique<SkinLoader>(doc.RootElement()->FirstChildElement("library_controllers"), MAX_WEIGHT_COUNT);

		std::unique_ptr<SkeletonLoader> skeletonLoader = std::make_unique<SkeletonLoader>();
		skeletonLoader->ExtractBoneData(doc.RootElement()->FirstChildElement("library_visual_scenes"), skinLoader->GetJointOrder(), joints);

		std::unique_ptr<GeometryLoader> geometryLoader = std::make_unique<GeometryLoader>();
		geometryLoader->ExtractAnimatedModelData(doc.RootElement()->FirstChildElement("library_geometries"), 
			skinLoader->GetSkinData(), MAX_WEIGHT_COUNT, meshes);
	}

}