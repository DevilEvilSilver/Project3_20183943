#pragma once
#include "Buffer.h"

#include <memory>

namespace Silver {

	class VertexArray
	{
	public:
		VertexArray();
		virtual ~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() const { return m_VertexBufferList; }
		const std::vector<std::shared_ptr<IndexBuffer>>& GetIndexBuffer() const { return m_IndexBufferList; }
		unsigned int GetIndicesCount() { return m_IndicesCount; }

	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBufferList;
		std::vector<std::shared_ptr<IndexBuffer>> m_IndexBufferList;
		unsigned int m_IndicesCount = 0;
	};

}
