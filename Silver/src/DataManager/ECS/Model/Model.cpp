#include "pch.h"
#include "Model.h"
#include "Utils/ColladaParser/ColladaLoader.h"

#include <filesystem>

namespace Silver {

	Model::Model(const std::string& filepath)
	{
		// Extract name & directory from filepath
		m_Directory = filepath;
		std::filesystem::path path = filepath;
		m_Name = path.stem().string();
	}

	Model::Model(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& meshes)
		: m_Name(name), m_Directory(""), m_Meshes(meshes)
	{
	}

	StaticModel::StaticModel(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& meshes)
		: Model(name, meshes)
	{
	}

	StaticModel::StaticModel(const std::string& filepath)
		: Model(filepath)
	{
		ColladaLoader::LoadStaticModel(filepath, m_Meshes);
	}

	AnimatedModel::AnimatedModel(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& meshes)
		: Model(name, meshes)
	{
		m_ModelType = ModelType::Animated;
	}

	AnimatedModel::AnimatedModel(const std::string& filepath)
		: Model(filepath)
	{
		m_ModelType = ModelType::Animated;
		ColladaLoader::LoadAnimatedModel(filepath, m_Joints, m_Meshes);
		AddAnimation(ColladaLoader::LoadAnimation(filepath));
	}

	AnimatedModel::~AnimatedModel()
	{
		m_AnimationList.clear();
	}

	void AnimatedModel::ClearAnimationList()
	{
		m_AnimationList.clear();
	}

	void AnimatedModel::AddAnimation(const std::vector<std::shared_ptr<Animation>>& list)
	{
		for (auto anim : list)
		{
			AddAnimation(anim);
		}
	}

	void AnimatedModel::AddAnimation(const std::shared_ptr<Animation>& anim)
	{
		if (IsExist(anim->GetName()))
			SV_CORE_ERROR("Animation {0} already exist in {1} !!!", anim->GetName(), m_Name);
		m_AnimationList[anim->GetName()] = anim;
	}

	bool AnimatedModel::IsExist(const std::string& name)
	{
		return m_AnimationList.find(name) != m_AnimationList.end();
	}

	const std::shared_ptr<Animation>& AnimatedModel::GetAnimation(const std::string& name)
	{
		if (!IsExist(name))
		{
			SV_CORE_ERROR("Animation {0} is not exist in {1} !!!", name, m_Name);
			return m_AnimationList[DEFAULT_ANIMATION];
		}	
		else if (name == "")
		{
			return m_AnimationList[DEFAULT_ANIMATION];
		}
		return m_AnimationList[name];
	}

}