#include "pch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace Silver {

	//tmp
	static unsigned int DataTypeToOpenGLType(DataType type)
	{
		switch (type)
		{
			case DataType::Float:	return GL_FLOAT;
			case DataType::Float2:	return GL_FLOAT;
			case DataType::Float3:	return GL_FLOAT;
			case DataType::Float4:	return GL_FLOAT;
			case DataType::Mat3:	return GL_FLOAT;
			case DataType::Mat4:	return GL_FLOAT;
			case DataType::Int:		return GL_INT;
			case DataType::Int2:	return GL_INT;
			case DataType::Int3:	return GL_INT;
			case DataType::Int4:	return GL_INT;
			case DataType::Bool:	return GL_BOOL;
		}

		SV_CORE_ERROR("Unknown DataType: Can't convert DataType to OpenGLType!");
		return 0;
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		if (!vertexBuffer->GetLayout().GetLayout().size())
		{
			SV_CORE_ERROR("Vertex layout empty!");
		}

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& attrib : layout)
		{
			glEnableVertexAttribArray(m_LayoutIndex);
			glVertexAttribPointer(
				m_LayoutIndex,
				attrib.GetComponentCount(),
				DataTypeToOpenGLType(attrib.type),
				attrib.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(attrib.offset));
			m_LayoutIndex++;
		}
		m_VertexBufferList.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}