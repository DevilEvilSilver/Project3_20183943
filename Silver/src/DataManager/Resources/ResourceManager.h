#pragma once
#include "Utils/Singleton.h"

#include <vector>
#include <memory>

namespace Silver {

	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		void LoadModel(const std::string& path);
		
	private:
		//std::vector<std::unique_ptr<Model>> m_ModelList;

	};

}