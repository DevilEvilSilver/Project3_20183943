#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "Camera/Camera.h"

namespace Silver {

	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& worldMatrix = glm::mat4(1.0f));
		
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};

}