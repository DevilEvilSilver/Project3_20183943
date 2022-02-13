#include "pch.h"
#include "Scene.h"
#include "DataManager/ECS/Entity.h"
#include "DataManager/ECS/Components.h"
#include "Renderer/Renderer.h"

namespace Silver {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	std::shared_ptr<Entity> Scene::CreateEntity(const std::string& name, bool hasTransform)
	{
		auto entity = std::make_shared<Entity>(m_Registry.create(), this);
		if (hasTransform)
			entity->AddComponent<TransformComponent>();
		auto& tag = entity->AddComponent<TagComponent>();
		tag = name.empty() ? "Entity" : name;
		return entity;
	}

	std::shared_ptr<Entity> Scene::CreateEntity(Entity& entity)
	{
		if ((entt::entity)entity == entt::null)
			return CreateEntity("Empty Entity");
		auto copyEntity = std::make_shared<Entity>(m_Registry.create(), this);
		auto& name = copyEntity->AddComponent<TagComponent>().Tag;
		name = entity.GetComponent<TagComponent>().Tag;
		if (entity.HasComponent<TransformComponent>())
		{
			auto& component = entity.GetComponent<TransformComponent>();
			auto& copyComponent = copyEntity->AddComponent<TransformComponent>();
			copyComponent.Translation = component.Translation;
			copyComponent.Rotation = component.Rotation;
			copyComponent.Scale = component.Scale;
		}
		if (entity.HasComponent<CameraComponent>())
		{
			auto& component = entity.GetComponent<CameraComponent>();
			auto& copyComponent = copyEntity->AddComponent<CameraComponent>(component.m_Camera->GetCameraType());
			copyComponent.Primary = false;
			copyComponent.FixedAspectRatio = component.FixedAspectRatio;
		}
		if (entity.HasComponent<StaticModelComponent>())
		{
			auto& component = entity.GetComponent<StaticModelComponent>();
			auto& copyComponent = copyEntity->AddComponent<StaticModelComponent>(component.m_StaticModel);
		}
		if (entity.HasComponent<AnimatedModelComponent>())
		{
			auto& component = entity.GetComponent<AnimatedModelComponent>();
			auto& copyComponent = copyEntity->AddComponent<AnimatedModelComponent>(component.m_AnimatedModel);
		}
		if (entity.HasComponent<Texture2DComponent>())
		{
			auto& component = entity.GetComponent<Texture2DComponent>();
			auto& copyComponent = copyEntity->AddComponent<Texture2DComponent>(component.m_Texture);
		}
		if (entity.HasComponent<ShaderComponent>())
		{
			auto& component = entity.GetComponent<ShaderComponent>();
			auto& copyComponent = copyEntity->AddComponent<ShaderComponent>(component.m_Shader);
		}
		if (entity.HasComponent<ScriptComponent>())
		{
			auto& component = entity.GetComponent<ScriptComponent>();
			copyEntity->AddComponent<ScriptComponent>().Bind(component.Instance);
		}

		return copyEntity;
	}

	void Scene::HideEntity(Entity& entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		tag = HIDED_ENTITY;
	}

	void Scene::DestroyEntity(Entity& entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::Clear()
	{
		m_Registry.clear();
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
			{
				return Entity{ entity, this };
			}
		}
		SV_CORE_ERROR("NO PRIMARY CAMERA !!!");
		return Entity{ entt::null, this };
	}

	void Scene::OnUpdateEditor(float deltaTime, const glm::mat4& viewProjectionMatrix)
	{
		//// RENDER (no need for update script, animator, ...)
		Renderer::BeginScene(viewProjectionMatrix);

		// Render static model
		{
			auto view = m_Registry.view<TransformComponent, StaticModelComponent, ShaderComponent, Texture2DComponent>();
			for (auto entity : view)
			{
				auto& [transform, model, shader, texture] =
					view.get<TransformComponent, StaticModelComponent, ShaderComponent, Texture2DComponent>(entity);
				shader.m_Shader->Bind();
				if (texture.m_Texture)
					texture.m_Texture->Bind();
				shader.m_Shader->SubmitUniformInt("u_Texture", 0);
				// Setting entity id for 2nd colorAttachment (might not need in actual game && CAN'T BE DONE WITH BATCHED RENDERING
				shader.m_Shader->SubmitUniformInt("u_EntityId", (int)(uint32_t)entity);

				Renderer::Submit(shader.m_Shader, model.m_StaticModel, transform.GetTransform());
			}
		}

		// Render animated model
		{
			auto view = m_Registry.view<TransformComponent, AnimatedModelComponent, ShaderComponent, Texture2DComponent>();
			for (auto entity : view)
			{
				auto& [transform, model, shader, texture] =
					view.get<TransformComponent, AnimatedModelComponent, ShaderComponent, Texture2DComponent>(entity);
				shader.m_Shader->Bind();
				if (texture.m_Texture)
					texture.m_Texture->Bind();
				shader.m_Shader->SubmitUniformInt("u_Texture", 0);
				// Setting entity id for 2nd colorAttachment (might not need in actual game && CAN'T BE DONE WITH BATCHED RENDERING
				shader.m_Shader->SubmitUniformInt("u_EntityId", (int)(uint32_t)entity); 
				//if (model.m_Animator->HasAnimation())
				//	shader.m_Shader->SubmitUniformMat4Array("u_JointTransform", model.m_AnimatedModel->GetJoints()->GetJointTransforms());

				Renderer::Submit(shader.m_Shader, model.m_AnimatedModel, transform.GetTransform());
			}
		}

		Renderer::EndScene();
	}

	void Scene::OnUpdateRuntime(float deltaTime)
	{
		//// UPDATE
		// Update scripts
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto entity : view)
			{
				auto& sc = view.get<ScriptComponent>(entity);
				if (sc.Instance)
				{
					sc.Instance->OnUpdate(deltaTime);
				}
			}
		}
		// Update animator
		{
			auto view = m_Registry.view<AnimatedModelComponent>();
			for (auto entity : view)
			{
				auto& component = view.get<AnimatedModelComponent>(entity);
				component.m_Animator->OnUpdate(deltaTime);
				component.ApplyPose();
			}
		}


		//// RENDER
		// Get main camera
		Entity mainCamera = GetPrimaryCameraEntity();
		if ((entt::entity)mainCamera != entt::null && mainCamera.HasComponent<CameraComponent>())	
			Renderer::BeginScene(mainCamera.GetComponent<CameraComponent>().m_Camera->GetViewProjectionMatrix());
		else
			Renderer::BeginScene();

		// Render static model
		{
			auto view = m_Registry.view<TransformComponent, StaticModelComponent, ShaderComponent, Texture2DComponent>();
			for (auto entity : view)
			{
				auto& [transform, model, shader, texture] =
					view.get<TransformComponent, StaticModelComponent, ShaderComponent, Texture2DComponent>(entity);
				shader.m_Shader->Bind();
				if (texture.m_Texture)
					texture.m_Texture->Bind();
				shader.m_Shader->SubmitUniformInt("u_Texture", 0);
				// Setting entity id for 2nd colorAttachment (might not need in actual game && CAN'T BE DONE WITH BATCHED RENDERING
				shader.m_Shader->SubmitUniformInt("u_EntityId", (int)(uint32_t)entity);

				Renderer::Submit(shader.m_Shader, model.m_StaticModel, transform.GetTransform());
			}
		}

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
				if (model.m_Animator->HasAnimation())
					shader.m_Shader->SubmitUniformMat4Array("u_JointTransform", model.m_AnimatedModel->GetJoints()->GetJointTransforms());

				Renderer::Submit(shader.m_Shader, model.m_AnimatedModel, transform.GetTransform());
			}
		}

		Renderer::EndScene();
	}

	void Scene::OnEvent(Event& e)
	{
		// Update scripts
		{
			auto view = m_Registry.view<ScriptComponent>();
			for (auto entity : view)
			{
				auto& sc = view.get<ScriptComponent>(entity);
				if (sc.Instance)
				{
					sc.Instance->OnEvent(e);
				}
			}
		}
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