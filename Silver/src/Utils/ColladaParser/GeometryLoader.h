#pragma once
#include "DataManager/Resources/Model/Model.h"

#include <memory>
#include <tinyxml2.h>

namespace Silver {
	
	class GeometryLoader
	{
	public:
		GeometryLoader() = default;
		~GeometryLoader();
		std::shared_ptr<Mesh> ExtractModelData(const std::string& filepath);

	private:
		std::shared_ptr<Mesh> ReadRawData(tinyxml2::XMLElement*& mesh);
		void AssembleVertices(tinyxml2::XMLElement*& mesh,
			const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& texCoords);

	private:
		std::vector<float> m_Vertices; // should consider using double
		std::vector<unsigned int> m_Indices;
	};

}