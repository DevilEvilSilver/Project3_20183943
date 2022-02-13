#include "pch.h"
#include "SceneSerializer.h"
#include "../ECS/Entity.h"
#include "../ECS/Components.h"
#include "../Resources/ResourceManager.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Silver {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene)
		:m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity& entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity; // Entity Id

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>();
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.m_Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera->GetProjectionType();
			out << YAML::Key << "CameraType" << YAML::Value << (int)camera->GetCameraType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera->GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera->GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera->GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera->GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera->GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera->GetOrthographicFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<StaticModelComponent>())
		{
			out << YAML::Key << "StaticModelComponent";
			out << YAML::BeginMap; // StaticModelComponent

			auto& smc = entity.GetComponent<StaticModelComponent>();
			out << YAML::Key << "Name" << YAML::Value << smc.m_StaticModel->GetName();

			out << YAML::EndMap; // StaticModelComponent
		}

		if (entity.HasComponent<AnimatedModelComponent>())
		{
			out << YAML::Key << "AnimatedModelComponent";
			out << YAML::BeginMap; // AnimatedModelComponent

			auto& amc = entity.GetComponent<AnimatedModelComponent>();
			out << YAML::Key << "Name" << YAML::Value << amc.m_AnimatedModel->GetName();

			out << YAML::EndMap; // AnimatedModelComponent
		}

		if (entity.HasComponent<Texture2DComponent>())
		{
			out << YAML::Key << "Texture2DComponent";
			out << YAML::BeginMap; // Texture2DComponent

			auto& t2Dc = entity.GetComponent<Texture2DComponent>();
			out << YAML::Key << "Name" << YAML::Value << t2Dc.m_Texture->GetName();

			out << YAML::EndMap; // Texture2DComponent
		}

		if (entity.HasComponent<ShaderComponent>())
		{
			out << YAML::Key << "ShaderComponent";
			out << YAML::BeginMap; // ShaderComponent

			auto& sc = entity.GetComponent<ShaderComponent>();
			out << YAML::Key << "Name" << YAML::Value << sc.m_Shader->GetName();

			out << YAML::EndMap; // ShaderComponent
		}

		out << YAML::EndMap; // Entity
	}

	static void SerializeLibrary(YAML::Emitter& out)
	{
		out << YAML::Key << "ModelLibrary" << YAML::BeginSeq;
		for (auto& model : ResourceManager::GetInstance()->m_ModelLibrary.GetLibrary())
		{
			if (model.first == DEFAULT_ANIMATED_MODEL || model.first == DEFAULT_STATIC_MODEL)
				continue;
			out << YAML::BeginMap;
			out << YAML::Key << "Filepath" << YAML::Value << model.second->GetDirectory();
			out << YAML::Key << "Type" << YAML::Value << (int)model.second->GetModelType();
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;

		out << YAML::Key << "TextureLibrary" << YAML::BeginSeq;
		for (auto& texture : ResourceManager::GetInstance()->m_TextureLibrary.GetLibrary())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Filepath" << YAML::Value << texture.second->GetDirectory();
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;

		out << YAML::Key << "ShaderLibrary" << YAML::BeginSeq;
		for (auto& shader : ResourceManager::GetInstance()->m_ShaderLibrary.GetLibrary())
		{
			if (shader.first == DEFAULT_SHADER)
				continue;
			out << YAML::BeginMap;
			out << YAML::Key << "Filepath" << YAML::Value << shader.second->GetDirectory();
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";

		out << YAML::Key << "Resources" << YAML::BeginMap;
		SerializeLibrary(out);
		out << YAML::EndMap;

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			if (entity.GetComponent<TagComponent>().Tag != HIDED_ENTITY)
				SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();

		SV_CORE_TRACE("Serialize scene to {0}", filepath);
	}

	void SceneSerializer::SerializeRunTime(const std::string& filepath)
	{
		// Not implemented
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			return false;
		}

		if (!data["Scene"])
			return false;

		//m_Scene->Clear(); // Optional
		std::string sceneName = data["Scene"].as<std::string>();
		SV_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto libraries = data["Resources"];
		if (libraries)
		{
			auto modelLibrary = libraries["ModelLibrary"];
			if (modelLibrary)
			{
				for (auto model : modelLibrary)
				{
					auto type = (Model::ModelType)model["Type"].as<int>();
					switch (type)
					{
					case Model::ModelType::Static:
						ResourceManager::GetInstance()->m_ModelLibrary.LoadStatic(model["Filepath"].as<std::string>());
						break;
					case Model::ModelType::Animated:
						ResourceManager::GetInstance()->m_ModelLibrary.LoadAnimated(model["Filepath"].as<std::string>());
						break;
					}
				}
			}

			auto textureLibrary = libraries["TextureLibrary"];
			if (textureLibrary)
			{
				for (auto texture : textureLibrary)
				{
					ResourceManager::GetInstance()->m_TextureLibrary.LoadTexture2D(texture["Filepath"].as<std::string>());
				}
			}

			auto shaderLibrary = libraries["ShaderLibrary"];
			if (shaderLibrary)
			{
				for (auto shader : shaderLibrary)
				{
					ResourceManager::GetInstance()->m_ShaderLibrary.Load(shader["Filepath"].as<std::string>());
				}
			}
		}

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				SV_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				std::shared_ptr<Entity> deserializedEntity = m_Scene->CreateEntity(name, false);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity->AddComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cameraProps = cameraComponent["Camera"];
					auto& cc = deserializedEntity->AddComponent<CameraComponent>((Camera::CameraType)cameraProps["CameraType"].as<int>());

					cc.m_Camera->SetProjectionType((Camera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.m_Camera->SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.m_Camera->SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.m_Camera->SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.m_Camera->SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.m_Camera->SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.m_Camera->SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto staticModelComponent = entity["StaticModelComponent"];
				if (staticModelComponent)
				{
					// Entities always have transforms
					auto& smc = deserializedEntity->AddComponent<StaticModelComponent>();
					smc.m_StaticModel = std::static_pointer_cast<StaticModel>
						(ResourceManager::GetInstance()->m_ModelLibrary.Get(staticModelComponent["Name"].as<std::string>()));
				}

				auto animatedModelComponent = entity["AnimatedModelComponent"];
				if (animatedModelComponent)
				{
					auto& amc = deserializedEntity->AddComponent<AnimatedModelComponent>();
					amc.m_AnimatedModel = std::static_pointer_cast<AnimatedModel>
						(ResourceManager::GetInstance()->m_ModelLibrary.Get(animatedModelComponent["Name"].as<std::string>()));
				}

				auto texture2DComponent = entity["Texture2DComponent"];
				if (texture2DComponent)
				{
					auto& t2Dc = deserializedEntity->AddComponent<Texture2DComponent>();
					t2Dc.m_Texture = std::static_pointer_cast<Texture2D>
						(ResourceManager::GetInstance()->m_TextureLibrary.Get(texture2DComponent["Name"].as<std::string>()));
				}

				auto shaderComponent = entity["ShaderComponent"];
				if (shaderComponent)
				{
					auto& sc = deserializedEntity->AddComponent<ShaderComponent>();
					sc.m_Shader = ResourceManager::GetInstance()->m_ShaderLibrary.Get(shaderComponent["Name"].as<std::string>());
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRunTime(const std::string& filepath)
	{
		// Not implemented
		return false;
	}

}