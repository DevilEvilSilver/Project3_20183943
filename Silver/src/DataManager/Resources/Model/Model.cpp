#include "pch.h"
#include "Model.h"
#include "Utils/ColladaParser/ColladaLoader.h"

#include <filesystem>

namespace Silver {

	Model::Model(const std::string& filepath)
	{
		// Extract name & directory from filepath
		m_Directory = filepath.substr(0, filepath.find_last_of('/'));
		std::filesystem::path path = filepath;
		m_Name = path.stem().string();
	}

	Model::Model(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& meshes)
		: m_Name(name), m_Directory(""), m_Meshes(meshes)
	{
	}

	StaticModel::StaticModel(const std::string& filepath)
		: Model(filepath)
	{
		ColladaLoader::LoadStaticModel(filepath, m_Meshes);
	}

	AnimatedModel::AnimatedModel(const std::string& filepath)
		: Model(filepath)
	{
		ColladaLoader::LoadAnimatedModel(filepath, m_Joints, m_Meshes);
	}

}