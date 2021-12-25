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
		void ExtractModelData(tinyxml2::XMLElement* node, 
			const std::vector<std::shared_ptr<VertexSkinData>>& skinData, std::vector<std::shared_ptr<Mesh>>& meshes);

	private:
		std::shared_ptr<Mesh> ReadRawData(tinyxml2::XMLElement*& mesh);
		void AssembleVertices(tinyxml2::XMLElement*& mesh,
			const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texCoords);

	private:
		std::vector<float> m_Vertices; // should consider using double
		std::vector<unsigned int> m_Indices;
	};

}