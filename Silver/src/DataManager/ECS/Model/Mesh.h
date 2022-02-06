#pragma once
#include "Renderer/VertexArray.h"

#include <vector>
#include <memory>

namespace Silver {

	class Mesh
	{
	public:
		Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer);
		Mesh(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers, const std::shared_ptr<IndexBuffer>& indexBuffer);
		~Mesh();

		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }

	private:
		std::shared_ptr<VertexArray> m_VertexArray;
	};

}