#include "pch.h"
#include "GeometryLoader.h"

#include <glm/glm.hpp>

namespace Silver {

	GeometryLoader::~GeometryLoader()
	{
		m_Vertices.clear();
		m_Indices.clear();
	}

	std::shared_ptr<Mesh> GeometryLoader::ExtractModelData(const std::string& filepath)
    {
		tinyxml2::XMLDocument doc;
		if (doc.LoadFile(filepath.c_str()))
		{
			SV_CORE_ERROR("Load failed");
		}

		tinyxml2::XMLElement* mesh = doc.RootElement()
			->FirstChildElement("library_geometries")
			->FirstChildElement("geometry")
			->FirstChildElement("mesh");

		return ReadRawData(mesh);
    }

	std::shared_ptr<Mesh> GeometryLoader::ReadRawData(tinyxml2::XMLElement*& mesh)
	{
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;

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
				m_Vertices.reserve(numVertices * 8);
				// Set vertices positions
				for (unsigned int i = 0; i < numVertices; i++)
				{
					m_Vertices.push_back(std::stof(positionData.substr(0, positionData.find(" "))));
					positionData.erase(0, positionData.find(" ") + 1);
					m_Vertices.push_back(std::stof(positionData.substr(0, positionData.find(" "))));
					positionData.erase(0, positionData.find(" ") + 1);
					m_Vertices.push_back(std::stof(positionData.substr(0, positionData.find(" "))));
					positionData.erase(0, positionData.find(" ") + 1);
					m_Vertices.push_back(0.0f);
					m_Vertices.push_back(0.0f);
					m_Vertices.push_back(0.0f);
					m_Vertices.push_back(0.0f);
					m_Vertices.push_back(0.0f);
				}
				source = source->NextSiblingElement("source");
			}
			// Get vertices normal
			else if (std::string(source->Attribute("id")) == normalID) {
				std::string normalData = source->FirstChildElement("float_array")->GetText();
				// Get number of normals
				unsigned int numNormals;
				source->FirstChildElement("float_array")->QueryUnsignedAttribute("count", &numNormals);
				numNormals /= 3;
				// Set vertices normal
				for (unsigned int i = 0; i < numNormals; i++)
				{
					float x = std::stof(normalData.substr(0, normalData.find(" ")));
					normalData.erase(0, normalData.find(" ") + 1);
					float y = std::stof(normalData.substr(0, normalData.find(" ")));
					normalData.erase(0, normalData.find(" ") + 1);
					float z = std::stof(normalData.substr(0, normalData.find(" ")));
					normalData.erase(0, normalData.find(" ") + 1);
					normals.push_back(glm::vec3(x, y, z));
				}
				source = source->NextSiblingElement("source");
			}
			// Get vertices texCoords
			else if (std::string(source->Attribute("id")) == texCoordID) {
				std::string texCoordData = source->FirstChildElement("float_array")->GetText();
				// Get number of texCoords
				unsigned int numTexCoords;
				source->FirstChildElement("float_array")->QueryUnsignedAttribute("count", &numTexCoords);
				numTexCoords /= 2;
				// Set vertices texCoords
				for (unsigned int i = 0; i < numTexCoords; i++)
				{
					float s = std::stof(texCoordData.substr(0, texCoordData.find(" ")));
					texCoordData.erase(0, texCoordData.find(" ") + 1);
					float t = std::stof(texCoordData.substr(0, texCoordData.find(" ")));
					texCoordData.erase(0, texCoordData.find(" ") + 1);
					texCoords.push_back(glm::vec2(s, t));
				}
				source = source->NextSiblingElement("source");
			}
			else {
				source = source->NextSiblingElement("source");
			}
		}
		
		AssembleVertices(mesh, normals, texCoords);

		float* vertices = new float[m_Vertices.size()];
		std::copy(m_Vertices.begin(), m_Vertices.end(), vertices);
		std::shared_ptr<Silver::VertexBuffer> VertexBuffer = std::make_shared<Silver::VertexBuffer>(vertices, sizeof(float) * m_Vertices.size());
		VertexBuffer->SetLayout({
			{ Silver::DataType::Float3, "a_Position"},
			{ Silver::DataType::Float3, "a_Normal"},
			{ Silver::DataType::Float2, "a_TexCoord"}
			});
		unsigned int* indices = new unsigned int[m_Indices.size()];
		std::copy(m_Indices.begin(), m_Indices.end(), indices);
		std::shared_ptr<Silver::IndexBuffer> IndexBuffer = std::make_shared<Silver::IndexBuffer>(indices, m_Indices.size());

		delete[]vertices;
		delete[]indices;

		return std::make_shared<Mesh>(VertexBuffer, IndexBuffer);
	}

	void GeometryLoader::AssembleVertices(tinyxml2::XMLElement*& mesh,
		const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texCoords)
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
		unsigned int vertexIndex;
		for (unsigned int i = 0; i < numIndices; i++) {
			//--------------haven't check or deal with duplicated vertex !!!-----------------
			// Process vertex
			vertexIndex = lstIndexData[i * typeCount];
			m_Indices.push_back(vertexIndex);
			m_Vertices[vertexIndex * 8 + 3] = normals[lstIndexData[i * typeCount + 1]].x;
			m_Vertices[vertexIndex * 8 + 4] = normals[lstIndexData[i * typeCount + 1]].y;
			m_Vertices[vertexIndex * 8 + 5] = normals[lstIndexData[i * typeCount + 1]].z;
			m_Vertices[vertexIndex * 8 + 6] = texCoords[lstIndexData[i * typeCount + 2]].s;
			m_Vertices[vertexIndex * 8 + 7] = texCoords[lstIndexData[i * typeCount + 2]].t;
		}
	}

}