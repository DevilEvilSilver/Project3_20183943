#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "Camera/OrthographicCamera.h"

namespace Silver {

	class Renderer
	{
	public:
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
		
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};

}