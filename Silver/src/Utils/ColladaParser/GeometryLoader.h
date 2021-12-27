#pragma once
#include "DataManager/Resources/Model/Model.h"
#include "SkinLoader.h"

#include <memory>
#include <glm/glm.hpp>
#include <tinyxml2.h>

namespace Silver {
	
	class ColladaVertex
	{
	public:
		ColladaVertex(unsigned int index, glm::vec3 pos, std::shared_ptr<VertexSkinData>& skinData)
			: m_Index(index), m_Position(pos), m_SkinData(skinData) 
		{
			m_DuplicateVertex = nullptr;
		}
		ColladaVertex(unsigned int index, glm::vec3 pos)
			: m_Index(index), m_Position(pos)
		{
			m_DuplicateVertex = nullptr;
		}
		~ColladaVertex() = default;

		bool IsSet() { return m_NormalIndex == -1 && m_TextureIndex == -1; }

		unsigned int m_Index;
		glm::vec3 m_Position;
		int m_NormalIndex = -1;
		int m_TextureIndex = -1;
		std::shared_ptr<ColladaVertex> m_DuplicateVertex;
		std::shared_ptr<VertexSkinData> m_SkinData;
	};

	class GeometryLoader
	{
	public:
		GeometryLoader() = default;
		~GeometryLoader();
		void ExtractAnimatedModelData(tinyxml2::XMLElement* node, const std::vector<std::shared_ptr<VertexSkinData>>& skinData, 
			unsigned int maxWeightCount, std::vector<std::shared_ptr<Mesh>>& meshes);
		void ExtractStaticModelData(tinyxml2::XMLElement* node, std::vector<std::shared_ptr<Mesh>>& meshes);	

	private:
		std::shared_ptr<Mesh> ReadRawData(tinyxml2::XMLElement*& mesh);
		void AssembleVertices(tinyxml2::XMLElement*& mesh);
		void ProcessVertex(unsigned int vertexIndex, unsigned int normalIndex, unsigned int texCoordIndex);
		void DealWithAlreadyProcessedVertex(unsigned int vertexIndex, unsigned int normalIndex, unsigned int texCoordIndex);
		std::shared_ptr<Mesh> ConvertToStaticMeshData();
		std::shared_ptr<Mesh> ConvertToAnimatedMeshData();

	private:
		// should consider using double instead of float
		std::vector<std::shared_ptr<ColladaVertex>> m_Vertices;
		std::vector<float> m_Normals;
		std::vector<float> m_TexCoords;
		std::vector<unsigned int> m_Indices;

		// Animated model data
		std::vector<std::shared_ptr<VertexSkinData>> m_SkinData;
		unsigned int m_MaxWeightCount = 0;
	};

}