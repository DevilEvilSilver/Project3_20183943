#pragma once
#include "DataManager/Resources/Model/Model.h"
#include "SkinLoader.h"

#include <memory>
#include <tinyxml2.h>

namespace Silver {
	
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
		void AssembleVertices(tinyxml2::XMLElement*& mesh,
			const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texCoords);
		std::shared_ptr<Mesh> AddSkinData(const std::vector<std::shared_ptr<VertexSkinData>>& skinData,
			unsigned int maxWeightCount, std::shared_ptr<Mesh>& mesh);

	private:
		// should consider using double instead of float
		std::vector<float> m_Vertices; 
		std::vector<unsigned int> m_Indices;
	};

}