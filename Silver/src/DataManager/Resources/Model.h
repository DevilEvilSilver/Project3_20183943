#pragma once
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

#include <string>
#include <memory>
#include <unordered_map>

// Temp fix !!!
struct aiScene;
struct aiNode;
struct aiMesh;

namespace Silver {

	class Mesh
	{
	public:
		Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer);
		~Mesh();

		const std::shared_ptr<VertexArray>& GetVertexArray() { return m_VertexArray; }

	private:
		//std::shared_ptr<VertexBuffer> m_VertexBuffer;
		//std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexArray> m_VertexArray;
	};

	class Model
	{
	public:
		Model(const std::string& filepath);
		Model(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& meshes);
		~Model();

		const std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return m_Meshes; }
		const std::string& GetName() { return m_Name; }

	private:
		void processNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);

	private:
		std::string m_Name;
		std::string m_Directory;
		std::vector<std::shared_ptr<Mesh>> m_Meshes;
	};

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

}