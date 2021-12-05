#include "pch.h"
#include "Model.h"

#include <filesystem>
#include <tinyxml2.h>

namespace Silver {

	Mesh::Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer)
		//:m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer)
	{
		m_VertexArray = std::make_shared<VertexArray>();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
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

		tinyxml2::XMLDocument doc;
		if (doc.LoadFile(filepath.c_str()))
		{
			SV_CORE_ERROR("Load failed");
		}
		else
		{
			tinyxml2::XMLElement* geometry = doc.RootElement()->FirstChildElement("library_geometries")->FirstChildElement("geometry");

			// Iterate through geometry elements 
			while (geometry != NULL) 
			{
				SV_CORE_TRACE("data name : {0}", geometry->Attribute("id"));

				geometry = geometry->NextSiblingElement("geometry");
			}
		}
		


		//Assimp::Importer import;
		//const aiScene* scene = import.ReadFile(filepath, aiProcess_Triangulate |
		//	aiProcess_FlipUVs);
		//if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		//	!scene->mRootNode)
		//{
		//	SV_CORE_ERROR("Assimp error: {0}", import.GetErrorString());
		//	return;
		//}
		
		
		//processNode(scene->mRootNode, scene);
	}

	Model::Model(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& meshes)
		: m_Name(name), m_Directory(""), m_Meshes(meshes)
	{
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		//// process all the node�s meshes (if any)
		//for (unsigned int i = 0; i < node->mNumMeshes; i++)
		//{
		//	aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		//	m_Meshes.push_back(processMesh(mesh, scene));
		//}
		//// then do the same for each of its children
		//for (unsigned int i = 0; i < node->mNumChildren; i++)
		//{
		//	processNode(node->mChildren[i], scene);
		//}
	}

	std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
		//vector<Texture> textures;
		
		//// process vertex positions, normals and texture coordinates
		//float* vertices = new float[mesh->mNumVertices * 8];
		//for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		//{
		//	vertices[i * 8] = mesh->mVertices[i].x;
		//	vertices[i * 8 + 1] = mesh->mVertices[i].y;
		//	vertices[i * 8 + 2] = mesh->mVertices[i].z;
		//	vertices[i * 8 + 3] = mesh->mNormals[i].x;
		//	vertices[i * 8 + 4] = mesh->mNormals[i].y;
		//	vertices[i * 8 + 5] = mesh->mNormals[i].z;
		//	if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		//	{
		//		vertices[i * 8 + 6] = mesh->mTextureCoords[0][i].x;
		//		vertices[i * 8 + 7] = mesh->mTextureCoords[0][i].y;
		//	}
		//	else
		//	{
		//		vertices[i * 8 + 6] = 0.0f;
		//		vertices[i * 8 + 7] = 0.0f;
		//	}
		//}
		//vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
		//vertexBuffer->SetLayout({
		//		{ Silver::DataType::Float3, "a_Position"},
		//		{ Silver::DataType::Float3, "a_Normal"},
		//		{ Silver::DataType::Float2, "a_TexCoord"}
		//	});

		//// process indices
		//unsigned int* indices = new unsigned int[mesh->mNumFaces * 3];
		//for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		//{
		//	aiFace face = mesh->mFaces[i];
		//	for (unsigned int j = 0; j < face.mNumIndices; j++)
		//		indices[i * 3 + j] = face.mIndices[j];
		//}
		//indexBuffer = std::make_shared<IndexBuffer>(indices, mesh->mNumFaces * 3);

		////process material
		////if (mesh->mMaterialIndex >= 0)
		////{
		////	[...]
		////}

		//delete[]vertices;
		//delete[]indices;
		return std::make_shared<Mesh>(vertexBuffer, indexBuffer);
	}
	
	AnimatedModel::AnimatedModel(const std::string& filepath)
		: Model(filepath)
	{
	}

}