#include "pch.h"
#include "GeometryLoader.h"

#include <glm/glm.hpp>

namespace Silver {

	GeometryLoader::~GeometryLoader()
	{
		m_Vertices.clear();
		m_Indices.clear();
	}

	void GeometryLoader::ExtractAnimatedModelData(tinyxml2::XMLElement* node, const std::vector<std::shared_ptr<VertexSkinData>>& skinData, 
		unsigned int maxWeightCount, std::vector<std::shared_ptr<Mesh>>& meshes)
    {
		m_SkinData = skinData;
		m_MaxWeightCount = maxWeightCount;

		tinyxml2::XMLElement* geometry = node->FirstChildElement("geometry");
		while (geometry != NULL)
		{
			unsigned int meshesCount = 0; // can delete
			tinyxml2::XMLElement* mesh = geometry->FirstChildElement("mesh");
			while (mesh != NULL)
			{
				meshes.push_back(ReadRawData(mesh));
				meshesCount++; // can delete
				mesh = mesh->NextSiblingElement("mesh");
			}
			if (meshesCount > 1)
				SV_CORE_TRACE("Model load with more than 1 meshes per geometry !!!"); // can delete
			geometry = geometry->NextSiblingElement("geometry");
		}
    }

	void GeometryLoader::ExtractStaticModelData(tinyxml2::XMLElement* node, std::vector<std::shared_ptr<Mesh>>& meshes)
	{
		tinyxml2::XMLElement* geometry = node->FirstChildElement("geometry");
		while (geometry != NULL)
		{
			unsigned int meshesCount = 0; // can delete
			tinyxml2::XMLElement* mesh = geometry->FirstChildElement("mesh");
			while (mesh != NULL)
			{
				meshes.push_back(ReadRawData(mesh));
				meshesCount++; // can delete
				mesh = mesh->NextSiblingElement("mesh");
			}
			if (meshesCount > 1)
				SV_CORE_TRACE("Model load with more than 1 meshes per geometry !!!"); // can delete
			geometry = geometry->NextSiblingElement("geometry");
		}
	}

	std::shared_ptr<Mesh> GeometryLoader::ReadRawData(tinyxml2::XMLElement*& mesh)
	{
		// Get source ID
		std::string positionID, normalID, texCoordID;
		std::string positionSource = mesh
			->FirstChildElement("vertices")
			->FirstChildElement("input")
			->Attribute("source");
		positionID = positionSource.substr(1);
		tinyxml2::XMLElement* input;
		if (mesh->FirstChildElement("triangles"))
		{
			input = mesh->FirstChildElement("triangles")->FirstChildElement("input");
		}
		else if (mesh->FirstChildElement("polylist")) {
			input = mesh->FirstChildElement("polylist")->FirstChildElement("input");
		}
		else {
			SV_CORE_ERROR("Unknown primitive type in geometry");
		}
		while (input != NULL) {
			// Get normal ID
			if (std::string(input->Attribute("semantic")) == "NORMAL") {
				std::string normalSource = input->Attribute("source");
				normalID = normalSource.substr(1);
				input = input->NextSiblingElement("input");
			}
			// Get texCoord ID
			else if (std::string(input->Attribute("semantic")) == "TEXCOORD") {
				std::string texCoordSource = input->Attribute("source");
				texCoordID = texCoordSource.substr(1);
				input = input->NextSiblingElement("input");
			}
			else {
				input = input->NextSiblingElement("input");
			}
		}

		// Iterate through source elements 
		tinyxml2::XMLElement* source = mesh->FirstChildElement("source");
		while (source != NULL) {
			// Get vertices position
			if (std::string(source->Attribute("id")) == positionID) {
				std::string positionData = source->FirstChildElement("float_array")->GetText();
				// Get number of vertices
				unsigned int numVertices;
				source->FirstChildElement("float_array")->QueryUnsignedAttribute("count", &numVertices);
				numVertices /= 3;
				m_Vertices.reserve(numVertices);
				// Set vertices positions
				if (m_MaxWeightCount != 0)
					for (unsigned int i = 0; i < numVertices; i++)
					{
						float x = std::stof(positionData.substr(0, positionData.find(" ")));
						positionData.erase(0, positionData.find(" ") + 1);
						float y = std::stof(positionData.substr(0, positionData.find(" ")));
						positionData.erase(0, positionData.find(" ") + 1);
						float z = std::stof(positionData.substr(0, positionData.find(" ")));
						positionData.erase(0, positionData.find(" ") + 1);
						m_Vertices.push_back(std::make_shared<ColladaVertex>(i, glm::vec3(x, y, z), m_SkinData[i]));
					}
				else
					for (unsigned int i = 0; i < numVertices; i++)
					{
						float x = std::stof(positionData.substr(0, positionData.find(" ")));
						positionData.erase(0, positionData.find(" ") + 1);
						float y = std::stof(positionData.substr(0, positionData.find(" ")));
						positionData.erase(0, positionData.find(" ") + 1);
						float z = std::stof(positionData.substr(0, positionData.find(" ")));
						positionData.erase(0, positionData.find(" ") + 1);
						m_Vertices.push_back(std::make_shared<ColladaVertex>(i, glm::vec3(x, y, z)));
					}
			}
			// Get vertices normal
			else if (std::string(source->Attribute("id")) == normalID) {
				std::string normalData = source->FirstChildElement("float_array")->GetText();
				// Get number of normals
				unsigned int numNormals;
				source->FirstChildElement("float_array")->QueryUnsignedAttribute("count", &numNormals);
				m_Normals.reserve(numNormals);
				// Set vertices normal
				for (unsigned int i = 0; i < numNormals / 3; i++)
				{
					m_Normals.push_back(std::stof(normalData.substr(0, normalData.find(" "))));
					normalData.erase(0, normalData.find(" ") + 1);
					m_Normals.push_back(std::stof(normalData.substr(0, normalData.find(" "))));
					normalData.erase(0, normalData.find(" ") + 1);
					m_Normals.push_back(std::stof(normalData.substr(0, normalData.find(" "))));
					normalData.erase(0, normalData.find(" ") + 1);
				}
			}
			// Get vertices texCoords
			else if (std::string(source->Attribute("id")) == texCoordID) {
				std::string texCoordData = source->FirstChildElement("float_array")->GetText();
				// Get number of texCoords
				unsigned int numTexCoords;
				source->FirstChildElement("float_array")->QueryUnsignedAttribute("count", &numTexCoords);
				m_TexCoords.reserve(numTexCoords);
				// Set vertices texCoords
				for (unsigned int i = 0; i < numTexCoords / 2; i++)
				{
					m_TexCoords.push_back(std::stof(texCoordData.substr(0, texCoordData.find(" "))));
					texCoordData.erase(0, texCoordData.find(" ") + 1);
					m_TexCoords.push_back(std::stof(texCoordData.substr(0, texCoordData.find(" "))));
					texCoordData.erase(0, texCoordData.find(" ") + 1);
				}
			}
			source = source->NextSiblingElement("source");
		}
		
		AssembleVertices(mesh);

		if (m_MaxWeightCount == 0)
			return ConvertToStaticMeshData();
		else
			return ConvertToAnimatedMeshData();
	}

	void GeometryLoader::AssembleVertices(tinyxml2::XMLElement*& mesh)
	{
		tinyxml2::XMLElement* primitve;
		if (mesh->FirstChildElement("triangles"))
		{
			primitve = mesh->FirstChildElement("triangles");
		}
		else if (mesh->FirstChildElement("polylist")) {
			primitve = mesh->FirstChildElement("polylist");
		}
		else {
			SV_CORE_ERROR("Unknown primitive type in geometry");
		}
	
		tinyxml2::XMLElement* input = primitve->FirstChildElement("input");
		unsigned int typeCount = 0;
		while (input != NULL)
		{
			typeCount++;
			input = input->NextSiblingElement("input");
		}

		// Get indices data
		std::string strIndexData = primitve->FirstChildElement("p")->GetText();
		std::vector<unsigned int> lstIndexData;
		size_t numIndexData = std::count(strIndexData.begin(), strIndexData.end(), ' ');
		lstIndexData.reserve(++numIndexData);
		for (unsigned int i = 0; i < numIndexData; i++)
		{
			lstIndexData.push_back(std::stoi(strIndexData.substr(0, strIndexData.find(" "))));
			strIndexData.erase(0, strIndexData.find(" ") + 1);
		}

		// Set indices data
		unsigned int numIndices = (unsigned int) numIndexData / typeCount;
		m_Indices.reserve(numIndices);
		for (unsigned int i = 0; i < numIndices; i++) {
			// Process vertex
			unsigned int vertexIndex = lstIndexData[i * typeCount];
			unsigned int normalIndex = lstIndexData[i * typeCount + 1];
			unsigned int texCoordIndex = lstIndexData[i * typeCount + 2];
			ProcessVertex(vertexIndex, normalIndex, texCoordIndex);
		}
	}

	void GeometryLoader::ProcessVertex(unsigned int vertexIndex, unsigned int normalIndex, unsigned int texCoordIndex)
	{
		if (m_Vertices[vertexIndex]->IsNotSet())
		{
			m_Vertices[vertexIndex]->m_NormalIndex = normalIndex;
			m_Vertices[vertexIndex]->m_TextureIndex = texCoordIndex;
			m_Indices.push_back(vertexIndex);
		}
		else
			DealWithAlreadyProcessedVertex(vertexIndex, normalIndex, texCoordIndex);
	}

	void GeometryLoader::DealWithAlreadyProcessedVertex(unsigned int vertexIndex, unsigned int normalIndex, unsigned int texCoordIndex)
	{
		if (m_Vertices[vertexIndex]->m_NormalIndex == normalIndex &&
			m_Vertices[vertexIndex]->m_TextureIndex == texCoordIndex)
			m_Indices.push_back(vertexIndex);
		else
		{
			std::shared_ptr<ColladaVertex> anotherVertex = m_Vertices[vertexIndex]->m_DuplicateVertex;
			if (anotherVertex != nullptr)
				DealWithAlreadyProcessedVertex(anotherVertex->m_Index, normalIndex, texCoordIndex);
			else
			{
				std::shared_ptr<ColladaVertex> duplicateVertex;
				if (m_MaxWeightCount != 0)
					duplicateVertex = std::make_shared<ColladaVertex>((unsigned int)m_Vertices.size(), 
						m_Vertices[vertexIndex]->m_Position, m_Vertices[vertexIndex]->m_SkinData);
				else
					duplicateVertex = std::make_shared<ColladaVertex>((unsigned int)m_Vertices.size(),
						m_Vertices[vertexIndex]->m_Position);
				duplicateVertex->m_NormalIndex = normalIndex;
				duplicateVertex->m_TextureIndex = texCoordIndex;
				m_Vertices[vertexIndex]->m_DuplicateVertex = duplicateVertex;
				m_Vertices.push_back(duplicateVertex);
				m_Indices.push_back(duplicateVertex->m_Index);
			}
		}
	}

	std::vector<std::shared_ptr<Silver::VertexBuffer>> GeometryLoader::GetBufferList()
	{
		float* vertices = new float[m_Vertices.size() * 8];
		for (unsigned int i = 0; i < m_Vertices.size(); ++i)
		{
			vertices[i * 8] = m_Vertices[i]->m_Position.x;
			vertices[i * 8 + 1] = m_Vertices[i]->m_Position.y;
			vertices[i * 8 + 2] = m_Vertices[i]->m_Position.z;
			vertices[i * 8 + 3] = m_Normals[m_Vertices[i]->m_NormalIndex * 3];
			vertices[i * 8 + 4] = m_Normals[m_Vertices[i]->m_NormalIndex * 3 + 1];
			vertices[i * 8 + 5] = m_Normals[m_Vertices[i]->m_NormalIndex * 3 + 2];
			vertices[i * 8 + 6] = m_TexCoords[m_Vertices[i]->m_TextureIndex * 2];
			vertices[i * 8 + 7] = m_TexCoords[m_Vertices[i]->m_TextureIndex * 2 + 1];
		}
		std::shared_ptr<Silver::VertexBuffer> VertexBuffer = std::make_shared<Silver::VertexBuffer>(vertices, sizeof(float) * m_Vertices.size() * 8);
		VertexBuffer->SetLayout({
			{ Silver::DataType::Float3, "a_Position"},
			{ Silver::DataType::Float3, "a_Normal"},
			{ Silver::DataType::Float2, "a_TexCoord"}
			});

		std::vector<std::shared_ptr<Silver::VertexBuffer>> BufferList;
		BufferList.push_back(VertexBuffer);
		delete[]vertices;

		return BufferList;
	}

	std::shared_ptr<Mesh> GeometryLoader::ConvertToStaticMeshData()
	{
		std::vector<std::shared_ptr<Silver::VertexBuffer>> BufferList = GetBufferList();

		unsigned int* indices = new unsigned int[m_Indices.size()];
		std::copy(m_Indices.begin(), m_Indices.end(), indices);
		std::shared_ptr<Silver::IndexBuffer> IndexBuffer = std::make_shared<Silver::IndexBuffer>(indices, m_Indices.size());
		delete[]indices;

		auto meshData = std::make_shared<Mesh>(BufferList, IndexBuffer);

		return meshData;
	}

	std::shared_ptr<Mesh> GeometryLoader::ConvertToAnimatedMeshData()
	{
		unsigned int* jointID = new unsigned int[m_Vertices.size() * 3];
		float* weight = new float[m_Vertices.size() * 3];
		for (unsigned int i = 0; i < m_Vertices.size(); ++i)
		{
			jointID[i * m_MaxWeightCount] = m_Vertices[i]->m_SkinData->GetJointIDList()[0];
			jointID[i * m_MaxWeightCount + 1] = m_Vertices[i]->m_SkinData->GetJointIDList()[1];
			jointID[i * m_MaxWeightCount + 2] = m_Vertices[i]->m_SkinData->GetJointIDList()[2];
			weight[i * m_MaxWeightCount] = m_Vertices[i]->m_SkinData->GetWeightList()[0];
			weight[i * m_MaxWeightCount + 1] = m_Vertices[i]->m_SkinData->GetWeightList()[1];
			weight[i * m_MaxWeightCount + 2] = m_Vertices[i]->m_SkinData->GetWeightList()[2];
		}
		std::shared_ptr<Silver::VertexBuffer> JointBuffer =
			std::make_shared<Silver::VertexBuffer>(jointID, sizeof(unsigned int) * m_Vertices.size() * 3);
		JointBuffer->SetLayout({
			{ Silver::DataType::Int3, "a_JointID"},
			});
		std::shared_ptr<Silver::VertexBuffer> WeightBuffer =
			std::make_shared<Silver::VertexBuffer>(weight, sizeof(float) * m_Vertices.size() * 3);
		WeightBuffer->SetLayout({
			{ Silver::DataType::Float3, "a_Weight"},
			});

		std::vector<std::shared_ptr<Silver::VertexBuffer>> BufferList = GetBufferList();
		BufferList.push_back(JointBuffer);
		BufferList.push_back(WeightBuffer);

		unsigned int* indices = new unsigned int[m_Indices.size()];
		std::copy(m_Indices.begin(), m_Indices.end(), indices);
		std::shared_ptr<Silver::IndexBuffer> IndexBuffer = std::make_shared<Silver::IndexBuffer>(indices, m_Indices.size());

		delete[]jointID;
		delete[]weight;
		delete[]indices;

		auto meshData = std::make_shared<Mesh>(BufferList, IndexBuffer);

		return meshData;
	}

}