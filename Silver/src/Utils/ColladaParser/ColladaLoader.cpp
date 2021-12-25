#include "pch.h"
#include "ColladaLoader.h"
#include "SkinLoader.h"
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

		const unsigned int MAX_WEIGHT_COUNT = 3;
		std::unique_ptr<SkinLoader> skinLoader = 
			std::make_unique<SkinLoader>(doc.RootElement()->FirstChildElement("library_controllers"), MAX_WEIGHT_COUNT);

		//SkinLoader skinLoader = new SkinLoader(node.getChild("library_controllers"), maxWeights);
		//SkinningData skinningData = skinLoader.extractSkinData();

		//SkeletonLoader jointsLoader = new SkeletonLoader(node.getChild("library_visual_scenes"), skinningData.jointOrder);
		//SkeletonData jointsData = jointsLoader.extractBoneData();

		std::unique_ptr<GeometryLoader> geometryLoader = std::make_unique<GeometryLoader>();
		geometryLoader->ExtractModelData(doc.RootElement()->FirstChildElement("library_geometries"), skinLoader->GetSkinData(), meshes);

		//GeometryLoader g = new GeometryLoader(node.getChild("library_geometries"), skinningData.verticesSkinData);
		//MeshData meshData = g.extractModelData();
	}

}