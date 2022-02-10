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
		std::shared_ptr<Camera> mainCamera = nullptr;
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
		if (!mainCamera)
		{
			SV_CORE_ERROR("NO PRIMARY CAMERA !!!");
			Renderer::BeginScene();
		}
		else
			Renderer::BeginScene(mainCamera->GetViewProjectionMatrix());

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
					shader.m_Shader->SubmitUniformMat4Array("u_JointTransform", model.m_Model->GetJoints()->GetJointTransforms());

				Renderer::Submit(shader.m_Shader, model.m_Model, transform.GetTransform());
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