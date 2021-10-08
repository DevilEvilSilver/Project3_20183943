#pragma once
#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Silver {

	class RenderCommand
	{
	public:
		void static SetClearColor(const glm::vec4& color);
		void static Clear();

		void static DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
	};

}