#include "pch.h"
#include "Renderer.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace Silver {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::Init()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::OnWindowResize(unsigned int width, unsigned int height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	}

	void Renderer::BeginScene(const glm::mat4& viewProjectionMat)
	{
		s_SceneData->ViewProjectionMatrix = viewProjectionMat;
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Mesh>& mesh, const glm::mat4 & worldMatrix)
	{
		shader->Bind();
		shader->SubmitUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SubmitUniformMat4("u_World", worldMatrix);

		auto vertexArray = mesh->GetVertexArray();
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		vertexArray->Unbind();
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Model>& model, const glm::mat4& worldMatrix)
	{
		shader->Bind();
		shader->SubmitUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SubmitUniformMat4("u_World", worldMatrix);

#ifdef SV_DEBUG
		if (model->GetMeshes().empty())
			SV_CORE_ERROR("Empty Model: {0}", model->GetName());
#endif // DEBUG

		for (auto& mesh : model->GetMeshes())
		{
			auto& vertexArray = mesh->GetVertexArray(); // NEED TO CHECK IF MESH IS EMPTY !!!!
			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
			vertexArray->Unbind();
		}
	}

}