#include "pch.h"
#include "ColladaLoader.h"
#include "SkinLoader.h"
#include "SkeletonLoader.h"
#include "GeometryLoader.h"
#include "AnimationLoader.h"

#include <tinyxml2.h>

namespace Silver {

	void ColladaLoader::LoadAnimatedModel(const std::string& filepath, 
		std::shared_ptr<Skeleton>& joints, std::vector<std::shared_ptr<Mesh>>& meshes)
	{
		tinyxml2::XMLDocument doc;
		if (doc.LoadFile(filepath.c_str()))
		{
			SV_CORE_ERROR("Load file failed {0}", filepath.c_str());
			return;
		}

		const unsigned int MAX_WEIGHT_COUNT = 3; // Currently support (1 vertex in effect of max to 3 joint)
		if (doc.RootElement()->FirstChildElement("library_controllers") == NULL)
		{
			SV_CORE_ERROR("file doesn't have library_controllers: {0}", filepath);
			return;
		}
		std::unique_ptr<SkinLoader> skinLoader = 
			std::make_unique<SkinLoader>(doc.RootElement()->FirstChildElement("library_controllers"), MAX_WEIGHT_COUNT);

		std::unique_ptr<SkeletonLoader> skeletonLoader = std::make_unique<SkeletonLoader>();
		skeletonLoader->ExtractBoneData(doc.RootElement()->FirstChildElement("library_visual_scenes"), skinLoader->GetJointOrder(), joints);

		std::unique_ptr<GeometryLoader> geometryLoader = std::make_unique<GeometryLoader>();
		geometryLoader->ExtractAnimatedModelData(doc.RootElement()->FirstChildElement("library_geometries"), 
			skinLoader->GetSkinData(), MAX_WEIGHT_COUNT, meshes);
	}

	void ColladaLoader::LoadStaticModel(const std::string& filepath, std::vector<std::shared_ptr<Mesh>>& meshes)
	{
		tinyxml2::XMLDocument doc;
		if (doc.LoadFile(filepath.c_str()))
		{
			SV_CORE_ERROR("Load file failed {0}", filepath.c_str());
			return;
		}

		std::unique_ptr<GeometryLoader> geometryLoader = std::make_unique<GeometryLoader>();
		geometryLoader->ExtractStaticModelData(doc.RootElement()->FirstChildElement("library_geometries"), meshes);
	}

	std::vector<std::shared_ptr<Animation>> ColladaLoader::LoadAnimation(const std::string& filepath)
	{
		tinyxml2::XMLDocument doc;
		if (doc.LoadFile(filepath.c_str()))
		{
			SV_CORE_ERROR("Load file failed {0}", filepath.c_str());
			return std::vector<std::shared_ptr<Animation>>();
		}

		if (doc.RootElement()->FirstChildElement("library_animations") == NULL)
		{
			SV_CORE_ERROR("file doesn't have library_animations: {0}", filepath);
			return std::vector<std::shared_ptr<Animation>>();
		}

		std::unique_ptr<AnimationLoader> animationLoader = std::make_unique<AnimationLoader>();
		return animationLoader->ExtractAnimattionData(doc.RootElement()->FirstChildElement("library_animations"), filepath);
	}

}