#pragma once
#include "DataManager/Resources/ResourceManager.h"
#include "DataManager/ECS/ScriptableEntity.h"
#include "DataManager/ECS/Animation/Animator.h"
#include "Renderer/Camera/Camera.h"
#include "Renderer/Camera/CameraOrbit.h"
#include "Renderer/Camera/CameraLookAt.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Silver {

	struct TagComponent
	{
		std::string Tag = "Entity";

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			:Tag(tag) {}

		operator const std::string& () const { return Tag; }
	};

	struct TransformComponent
	{
		// default value for blender import
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { glm::radians(-90.0f), 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			:Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::mat4_cast(glm::quat(Rotation))
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct StaticModelComponent
	{
		std::shared_ptr<StaticModel> m_StaticModel;

		StaticModelComponent() { m_StaticModel = std::static_pointer_cast<StaticModel>
			(ResourceManager::GetInstance()->m_ModelLibrary.Get(DEFAULT_STATIC_MODEL)); }
		StaticModelComponent(const StaticModelComponent&) = default;
		StaticModelComponent(const std::shared_ptr<StaticModel>& model)
			:m_StaticModel(model) {}
	};

	struct AnimatedModelComponent
	{
		std::shared_ptr<AnimatedModel> m_AnimatedModel;
		std::shared_ptr<Animator> m_Animator;

		AnimatedModelComponent() 
		{ 
			m_AnimatedModel = std::static_pointer_cast<AnimatedModel>(ResourceManager::GetInstance()->m_ModelLibrary.Get(DEFAULT_ANIMATED_MODEL));
			m_Animator = std::make_shared<Animator>(); 	
		}
		AnimatedModelComponent(const AnimatedModelComponent&) = default;
		AnimatedModelComponent(const std::shared_ptr<AnimatedModel>& model)
			:m_AnimatedModel(model) 
		{
			m_Animator = std::make_shared<Animator>();
			m_Animator->BindAnimation(m_AnimatedModel->GetAnimation(DEFAULT_ANIMATION));
		}

		void BindAnimation(const std::string& anim)
		{
			m_Animator->BindAnimation(m_AnimatedModel->GetAnimation(anim));
		}

		void UnbindAnimation()
		{
			//m_Animator->UnbindAnimation();
			m_Animator->BindAnimation(m_AnimatedModel->GetAnimation(DEFAULT_ANIMATION)); // Because get nullptr animation is currently dangerous
		}

		void ApplyPose()
		{
			m_Animator->ApplyPoseToJoints(m_AnimatedModel->GetJoints()->GetHeadJoint(), glm::mat4(1.0f));
		}
	};

	struct Texture2DComponent
	{
		std::shared_ptr<Texture2D> m_Texture;

		Texture2DComponent() = default;
		Texture2DComponent(const Texture2DComponent&) = default;
		Texture2DComponent(const std::shared_ptr<Texture2D>& texture)
			:m_Texture(texture) {}
	};

	// consider removing this as a component
	struct ShaderComponent
	{
		std::shared_ptr<Shader> m_Shader;

		ShaderComponent() {
			m_Shader = (ResourceManager::GetInstance()->m_ShaderLibrary.Get(DEFAULT_SHADER)); }
		ShaderComponent(const ShaderComponent&) = default;
		ShaderComponent(const std::shared_ptr<Shader>& shader)
			:m_Shader(shader) {}
	};

	struct CameraComponent
	{
		std::shared_ptr<Camera> m_Camera;
		bool Primary = false;
		bool FixedAspectRatio = false;

		CameraComponent() { m_Camera = std::make_shared<Camera>(); }
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const std::shared_ptr<Camera>& camera)
			:m_Camera(camera) {}
		CameraComponent(const Camera::CameraType& type)
		{
			switch (type)
			{
			case Camera::CameraType::None:
				m_Camera = std::make_shared<Camera>();
				break;
			case Camera::CameraType::Orbit:
				m_Camera = std::make_shared<CameraOrbit>();
				break;
			case Camera::CameraType::LookAt:
				m_Camera = std::make_shared<CameraLookAt>();
				break;
			}
		}
	};

	struct ScriptComponent
	{
		std::shared_ptr<ScriptableEntity> Instance = nullptr;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;

		void Bind(const std::shared_ptr<ScriptableEntity>& se)
		{
			Instance = se;
			Instance->OnAttach();
		}

		void Unbind()
		{
			Instance = nullptr;
			Instance->OnDetach();
		}
	};

}