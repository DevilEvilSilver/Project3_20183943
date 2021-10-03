#include "pch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "OpenGL/OpenGLBuffer.h"

namespace Silver {

	VertexBuffer * VertexBuffer::Create(float * vertices, unsigned int size)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::None:
			SV_CORE_ERROR("None rendererAPI is selected");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}	

		SV_CORE_ERROR("None rendererAPI unknown!");
		return nullptr;
	}

	IndexBuffer * IndexBuffer::Create(unsigned int * indices, unsigned int size)
	{
		switch (Renderer::GetRendererAPI())
		{
		case RendererAPI::None:
			SV_CORE_ERROR("None rendererAPI is selected");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
		}

		SV_CORE_ERROR("None rendererAPI unknown!");
		return nullptr;
	}
}