#include "pch.h"
#include "Mesh.h"

namespace Silver {

	Mesh::Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer)
		//:m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer)
	{
		m_VertexArray = std::make_shared<VertexArray>();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
		m_VertexArray->Unbind();
	}

	Mesh::~Mesh()
	{
	}

}