#pragma once
#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Silver {

	class RenderCommand
	{
	public:
		void static SetViewPort(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
		void static SetClearColor(const glm::vec4& color);
		void static Clear();

		void static DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
	};

}