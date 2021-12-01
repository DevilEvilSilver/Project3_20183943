#include "pch.h"
#include "ResourceManager.h"

namespace Silver {

	void ModelLibrary::Add(const std::shared_ptr<Model>& model)
	{
		auto& name = model->GetName();
		Add(name, model);
	}

	void ModelLibrary::Add(const std::string& name, const std::shared_ptr<Model>& model)
	{
		if (IsExist(name))
			SV_CORE_ERROR("Model {0} already exist in ModelLibrary !!!", name);
		m_Models[name] = model;
	}

	std::shared_ptr<Model> ModelLibrary::Load(const std::string& filepath)
	{
		auto model = std::make_shared<Model>(filepath);
		Add(model);
		return model;
	}

	std::shared_ptr<Model> ModelLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto model = std::make_shared<Model>(filepath);
		Add(name, model);
		return model;
	}

	std::shared_ptr<Model> ModelLibrary::Get(const std::string& name)
	{
		if (!IsExist(name))
			SV_CORE_ERROR("Model {0} does not exist in ModelLibrary !!!", name);
		return m_Models[name];
	}
	bool ModelLibrary::IsExist(const std::string& name) const
	{
		return m_Models.find(name) != m_Models.end();
	}


	void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader>& shader)
	{
		if (IsExist(name))
			SV_CORE_ERROR("Shader {0} already exist in ShaderLibrary !!!", name);
		m_Shaders[name] = shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = std::make_shared<Shader>(filepath);
		Add(shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = std::make_shared<Shader>(filepath);
		Add(name, shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		if (!IsExist(name))
			SV_CORE_ERROR("Shader {0} does not exist in ShaderLibrary !!!", name);
		return m_Shaders[name];
	}

	bool ShaderLibrary::IsExist(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}