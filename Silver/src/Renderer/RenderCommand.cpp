#include "pch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace Silver {

	void RenderCommand::SetClearColor(const glm::vec4 & color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndicesCount(), GL_UNSIGNED_INT, nullptr);
	}

}