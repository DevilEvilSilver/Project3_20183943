#pragma once
#include "Mesh.h"
#include "Skeleton.h"

#include <string>
#include <memory>
#include <unordered_map>

namespace Silver {

	class Model
	{
	public:
		Model(const std::string& filepath);
		Model(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& meshes);
		virtual ~Model() = default;
		
		virtual const std::string& GetName() { return m_Name; }
		virtual const std::string& GetDirectory() { return m_Directory; }
		virtual const std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return m_Meshes; }

	protected:
		std::string m_Name;
		std::string m_Directory;
		std::vector<std::shared_ptr<Mesh>> m_Meshes;
	};

	class StaticModel : public Model
	{
	public:
		StaticModel(const std::string& filepath);
		~StaticModel() = default;
	};

	class AnimatedModel : public Model
	{
	public:
		AnimatedModel(const std::string& filepath);
		~AnimatedModel() = default;

		const std::shared_ptr<Skeleton>& GetJoints() { return m_Joints; }

	private:
		std::shared_ptr<Skeleton> m_Joints;
	};

}