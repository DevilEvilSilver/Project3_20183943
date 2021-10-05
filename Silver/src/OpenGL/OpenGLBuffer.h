#pragma once
#include "Renderer/Buffer.h"

#include <glad/glad.h>

namespace Silver {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, unsigned int size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const VertexLayout& layout) override { m_Layout = layout; }

		virtual const VertexLayout& GetLayout() const override { return m_Layout; }

	private:
		uint32_t m_RendererID;
		VertexLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int * indices, unsigned int count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual unsigned int GetCount() const override { return m_Count; }

	private:
		uint32_t m_RendererID;
		unsigned int m_Count;
	};

}