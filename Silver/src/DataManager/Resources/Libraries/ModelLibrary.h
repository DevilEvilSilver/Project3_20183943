#pragma once
#include "DataManager/ECS/Model/Model.h"

#include <string>
#include <unordered_map>
#include <memory>

namespace Silver {

	class ModelLibrary
	{
	public:
		ModelLibrary();

		void Add(const std::shared_ptr<Model>& model);
		void Add(const std::string& name, const std::shared_ptr<Model>& model);
		void Remove(const std::string& name);
		std::shared_ptr<StaticModel> LoadStatic(const std::string& filepath);
		std::shared_ptr<StaticModel> LoadStatic(const std::string& name, const std::string& filepath);
		std::shared_ptr<AnimatedModel> LoadAnimated(const std::string& filepath);
		std::shared_ptr<AnimatedModel> LoadAnimated(const std::string& name, const std::string& filepath);

		std::shared_ptr<Model> Get(const std::string& name);
		const std::unordered_map<std::string, std::shared_ptr<Model>>& GetLibrary() { return m_Models; }

		bool IsExist(const std::string& name) const;

	private:
		std::unordered_map<std::string, std::shared_ptr<Model>> m_Models;
	};

#define DEFAULT_STATIC_MODEL "default static model"
#define DEFAULT_ANIMATED_MODEL "default animated model"

}