#pragma once
#include "DataManager/ECS/Model/Model.h"
#include "Shader.h"
#include "Camera/Camera.h"

namespace Silver {

	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(unsigned int width, unsigned int height);

		static void BeginScene(const std::shared_ptr<Camera>& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Mesh>& mesh, const glm::mat4& worldMatrix = glm::mat4(1.0f));
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Model>& model, const glm::mat4& worldMatrix = glm::mat4(1.0f));
		
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};

}