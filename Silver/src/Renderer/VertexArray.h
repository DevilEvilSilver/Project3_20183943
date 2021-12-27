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
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffer() const { return m_VertexBufferList; }
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		unsigned int m_LayoutIndex = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBufferList;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}
