#pragma once
#include "DataManager/ECS/ScriptableEntity.h"
#include "DataManager/Resources/Model/Model.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera/CameraLookAt.h"

#include <string>
#include <glm/glm.hpp>

namespace Silver {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			:Tag(tag) {}

		operator const std::string& () const { return Tag; }
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			:Transform(transform) {}

		operator const glm::mat4& () const { return Transform; }
	};

	struct StaticModelComponent
	{
		std::shared_ptr<StaticModel> m_Model;

		StaticModelComponent() = default;
		StaticModelComponent(const StaticModelComponent&) = default;
		StaticModelComponent(const std::shared_ptr<StaticModel>& model)
			:m_Model(model) {}
	};

	struct AnimatedModelComponent
	{
		std::shared_ptr<AnimatedModel> m_Model;

		AnimatedModelComponent() = default;
		AnimatedModelComponent(const AnimatedModelComponent&) = default;
		AnimatedModelComponent(const std::shared_ptr<AnimatedModel>& model)
			:m_Model(model) {}
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

		ShaderComponent() = default;
		ShaderComponent(const ShaderComponent&) = default;
		ShaderComponent(const std::shared_ptr<Shader>& shader)
			:m_Shader(shader) {}
	};

	struct CameraComponent
	{
		std::shared_ptr<Camera> m_Camera;
		bool Primary = false;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const std::shared_ptr<Camera>& camera)
			:m_Camera(camera) {}
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