#include "pch.h"
#include "Scene.h"
#include "DataManager/ECS/Entity.h"
#include "DataManager/ECS/Components.h"
#include "Renderer/Renderer.h"

namespace Silver {

	Scene::Scene()
	{
		
#ifdef ENTT_EXAMPLE 
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		if (m_Registry.all_of<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
		}
#endif

	}

	Scene::~Scene()
	{
	}

	std::shared_ptr<Entity> Scene::CreateEntity(const std::string& name)
	{
		auto entity = std::make_shared<Entity>(m_Registry.create(), this);
		entity->AddComponent<TransformComponent>();
		auto& tag = entity->AddComponent<TagComponent>();
		tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(float deltaTime)
	{
		// Get main camera
		std::shared_ptr<Camera> mainCamera;
		{
			auto view = m_Registry.view<CameraComponent>();
			for (auto entity : view)
			{
				auto& camera = view.get<CameraComponent>(entity);
				if (camera.Primary)
				{
					mainCamera = camera.m_Camera;
					break;
				}
			}
		}
		Renderer::BeginScene(mainCamera);

		// Render animated model
		{
			auto view = m_Registry.view<TransformComponent, AnimatedModelComponent, ShaderComponent, Texture2DComponent>();
			for (auto entity : view)
			{
				auto& [transform, model, shader, texture] =
					view.get<TransformComponent, AnimatedModelComponent, ShaderComponent, Texture2DComponent>(entity);
				shader.m_Shader->Bind();
				texture.m_Texture->Bind();
				shader.m_Shader->SubmitUniformInt("u_Texture", 0);

				Renderer::Submit(shader.m_Shader, model.m_Model, transform);
			}
		}

		Renderer::EndScene();
	}

	void Scene::OnViewportResize(float width, float height)
	{
		if (height == 0)
			return;
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize camera
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (camera.FixedAspectRatio)
			{
				camera.m_Camera->ResizeAspectRatio((float)width, (float)height);
			}
		}
	}

}