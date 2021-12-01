#pragma once
#include "Utils/Singleton.h"
#include "Model/Model.h"
#include "Renderer/Shader.h"

#include <string>
#include <unordered_map>
#include <memory>

namespace Silver {

	class ModelLibrary
	{
	public:
		void Add(const std::shared_ptr<Model>& model);
		void Add(const std::string& name, const std::shared_ptr<Model>& model);
		std::shared_ptr<Model> Load(const std::string& filepath);
		std::shared_ptr<Model> Load(const std::string& name, const std::string& filepath);

		std::shared_ptr<Model> Get(const std::string& name);

		bool IsExist(const std::string& name) const;

	private:
		std::unordered_map<std::string, std::shared_ptr<Model>> m_Models;
	};


	class ShaderLibrary
	{
	public:
		void Add(const std::shared_ptr<Shader>& shader);
		void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
		std::shared_ptr<Shader> Load(const std::string& filepath);
		std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

		std::shared_ptr<Shader> Get(const std::string& name);

		bool IsExist(const std::string& name) const;

	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	};

}