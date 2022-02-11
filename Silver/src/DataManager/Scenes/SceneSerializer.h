#pragma once
#include "Scene.h"

#include <string>
#include <memory>

namespace Silver {

	class SceneSerializer
	{
	public:
		SceneSerializer(const std::shared_ptr<Scene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeRunTime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRunTime(const std::string& filepath);

	private:
		std::shared_ptr<Scene> m_Scene;
	};

}