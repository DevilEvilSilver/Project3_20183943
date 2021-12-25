#include "pch.h"
#include "Model.h"
#include "Utils/ColladaParser/ColladaLoader.h"

#include <filesystem>

namespace Silver {

	Mesh::Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer)
		//:m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer)
	{
		m_VertexArray = std::make_shared<VertexArray>();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
		m_VertexArray->Unbind();
	}

	Mesh::~Mesh()
	{
	}

	Model::Model(const std::string& filepath)
	{
		// Extract name & directory from filepath
		m_Directory = filepath.substr(0, filepath.find_last_of('/'));
		std::filesystem::path path = filepath;
		m_Name = path.stem().string();
	}

	Model::Model(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& meshes)
		: m_Name(name), m_Directory("")
	{
	}
	
	AnimatedModel::AnimatedModel(const std::string& filepath)
		: Model(filepath)
	{
		ColladaLoader::LoadAnimatedModel(filepath, m_Joints, m_Meshes);
	}

}