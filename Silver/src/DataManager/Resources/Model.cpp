#include "pch.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Silver {

	Mesh::Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer)
		:m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer)
	{
		m_VertexArray = std::make_shared<VertexArray>();
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

	Mesh::~Mesh()
	{
	}

	Model::Model(const std::string& filepath)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(filepath, aiProcess_Triangulate |
			aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
			!scene->mRootNode)
		{
			SV_CORE_ERROR("Assimp error: {0}", import.GetErrorString());
			return;
		}
		std::string m_Directory = filepath.substr(0, filepath.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node’s meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(processMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::shared_ptr<Silver::VertexBuffer> vertexBuffer;
		std::shared_ptr<Silver::IndexBuffer> indexBuffer;
		//vector<Texture> textures;
		
		// process vertex positions, normals and texture coordinates
		float* vertices = new float[mesh->mNumVertices * 8];
		unsigned int count = 0;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			vertices[count++] = mesh->mVertices[i].x;
			vertices[count++] = mesh->mVertices[i].y;
			vertices[count++] = mesh->mVertices[i].z;
			vertices[count++] = mesh->mNormals[i].x;
			vertices[count++] = mesh->mNormals[i].y;
			vertices[count++] = mesh->mNormals[i].z;
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				vertices[count++] = mesh->mTextureCoords[0][i].x;
				vertices[count++] = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertices[count++] = 0.0f;
				vertices[count++] = 0.0f;
			}
		}
		vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));

		// process indices
		unsigned int* indices = new unsigned int[mesh->mNumFaces * 3];
		count = 0;
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices[count++] = face.mIndices[j];
		}
		indexBuffer = std::make_shared<IndexBuffer>(indices, mesh->mNumFaces * 3);

		//process material
		//if (mesh->mMaterialIndex >= 0)
		//{
		//	[...]
		//}

		return std::make_shared<Mesh>(vertexBuffer, indexBuffer);
	}

	Model::~Model()
	{
	}

	void ModelLibrary::Add(const std::shared_ptr<Model>& model)
	{
		auto& name = model->GetName();
		Add(name, model);
	}

	void ModelLibrary::Add(const std::string& name, const std::shared_ptr<Model>& model)
	{
		if (IsExist(name))
			SV_CORE_ERROR("Shader {0} already exist in ShaderLibrary !!!", name);
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
			SV_CORE_ERROR("Shader {0} does not exist in ShaderLibrary !!!", name);
		return m_Models[name];
	}
	bool ModelLibrary::IsExist(const std::string& name) const
	{
		return m_Models.find(name) != m_Models.end();
	}

}