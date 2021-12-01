#pragma once
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"

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
		std::shared_ptr<VertexArray> m_VertexArray;
	};

	class Model
	{
	public:
		Model(const std::string& filepath);
		Model(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& meshes);
		virtual ~Model();

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



}