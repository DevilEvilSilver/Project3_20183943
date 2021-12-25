#pragma once
#include "Renderer/VertexArray.h"

#include <memory>

namespace Silver {

	class Mesh
	{
	public:
		Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer);
		~Mesh();

		const std::shared_ptr<VertexArray>& GetVertexArray() { return m_VertexArray; }

	private:
		std::shared_ptr<VertexArray> m_VertexArray;
	};

}