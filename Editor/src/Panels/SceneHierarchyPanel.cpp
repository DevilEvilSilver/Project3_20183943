#include "SceneHierarchyPanel.h"
#include "DataManager/Resources/ResourceManager.h"
#include "DataManager/ECS/Components.h"
#include "Renderer/Camera/CameraOrbit.h"
#include "Renderer/Camera/CameraLookAt.h"
#include "../Styles/ImGuiStyles.h"

#include <string>
#include <filesystem>
#include <imgui.h>
#include <imgui_internal.h>

namespace Silver {

	extern const std::filesystem::path g_AssetPath;

	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& scene)
		:m_Context(scene)
	{
	}

	void SceneHierarchyPanel::SetContext(const std::shared_ptr<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = { entt::null, m_Context.get() };
		m_CopiededContext = { entt::null, m_Context.get() };
	}

	void SceneHierarchyPanel::SetSelectedEntity(const Entity& entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entity)
		{
			Entity e { entity, m_Context.get() };
			DrawEntityNode(e);
		});

		// left click on panel
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = { entt::null, m_Context.get() };

		// right click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			if ((entt::entity)m_CopiededContext != entt::null)
			if (ImGui::MenuItem("Paste Entity"))
				m_Context->CreateEntity(m_CopiededContext);

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Copy Entity"))
				m_CopiededContext = entity;

			if (ImGui::MenuItem("Hide Entity"))
				m_Context->HideEntity(entity);

			if (ImGui::MenuItem("Destroy Entity"))
				entityDeleted = true;
			
			ImGui::EndPopup();
		}

		if (opened)
		{
			for (unsigned int i = 0; i < 10; ++i)
			{
				ImGui::Text("%s", tag.c_str());
			}
			ImGui::TreePop();
		}

		//Destroy Entity
		if (entityDeleted)
		{
			if (m_SelectionContext == entity)
				m_SelectionContext = { entt::null, m_Context.get() };
			m_Context->DestroyEntity(entity);
		}	
	}

	template <typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity& entity, UIFunction uiFunction)
	{
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed 
				| ImGuiTreeNodeFlags_AllowItemOverlap;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight}))
			{
				ImGui::OpenPopup("ComponentsSettings");
			}
			ImGui::PopStyleVar();

			bool componentRemoved = false;
			if (ImGui::BeginPopup("ComponentsSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					componentRemoved = true;

				ImGui::EndPopup();
			}

			if (opened)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (componentRemoved)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity& entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			if (tag.length() < sizeof(buffer))
				strcpy_s(buffer, sizeof(buffer), tag.c_str());
			else
			{
				SV_CORE_ERROR("Tag name too long (should be < 256): {0}", tag);
				tag = "Entity";
			}
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Transform"))
			{
				m_SelectionContext.AddComponent<TransformComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Camera"))
			{
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("StaticModel"))
			{
				m_SelectionContext.AddComponent<StaticModelComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("AnimatedModel"))
			{
				m_SelectionContext.AddComponent<AnimatedModelComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Texture2D"))
			{
				m_SelectionContext.AddComponent<Texture2DComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Shader"))
			{
				m_SelectionContext.AddComponent<ShaderComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Script"))
			{
				m_SelectionContext.AddComponent<ScriptComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DragFloat3("Position", component.Translation, glm::vec3(0.0f), 70.0f);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DragFloat3("Rotation", rotation, glm::vec3(-90.0f, 0.0f, 0.0f), 70.f);
			component.Rotation = glm::radians(rotation);
			DragFloat3("Scale", component.Scale, glm::vec3(1.0f), 70.0f);
			if (ImGui::Button("Reset"))
			{
				component.Translation = { 0.0f, 0.0f, 0.0f };
				component.Rotation = { glm::radians(-90.0f), 0.0f, 0.0f };
				component.Scale = { 1.0f, 1.0f, 1.0f };
			}
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.m_Camera;

			// Have to check other camera primary attribute
			ImGui::Checkbox("Primary", &component.Primary);
			ImGui::Checkbox("Fixed AspectRatio", &component.FixedAspectRatio);

			const char* projectionTypeString[] = { "Perspective", "Orthographic" };
			const char* currProjectionTypeString = projectionTypeString[(int)camera->GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currProjectionTypeString))
			{
				for (unsigned int i = 0; i < 2; ++i)
				{
					bool isSelected = currProjectionTypeString == projectionTypeString[i];
					if (ImGui::Selectable(projectionTypeString[i], isSelected))
					{
						currProjectionTypeString = projectionTypeString[i];
						camera->SetProjectionType((Camera::ProjectionType)i);
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}

				}

				ImGui::EndCombo();
			}

			const char* cameraTypeString[] = { "None", "Orbit", "LookAt" };
			const char* currCameraTypeString = cameraTypeString[(int)camera->GetCameraType()];
			if (ImGui::BeginCombo("Type", currCameraTypeString))
			{
				for (unsigned int i = 0; i < 3; ++i)
				{
					bool isSelected = currCameraTypeString == cameraTypeString[i];
					if (ImGui::Selectable(cameraTypeString[i], isSelected))
					{
						switch (i)
						{
						case 0:
							camera = std::make_shared<Camera>();
							break;
						case 1:
							camera = std::make_shared<CameraOrbit>();
							break;
						case 2:
							camera = std::make_shared<CameraLookAt>();
							break;
						}
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}

				}

				ImGui::EndCombo();
			}

			if (camera->GetProjectionType() == Camera::ProjectionType::Perspective)
			{
				float perspectiveFOV = camera->GetPerspectiveFOV();
				if (ImGui::DragFloat("FOV", &perspectiveFOV, 0.2f, 1.0f, 179.0f))
					camera->SetPerspectiveFOV(perspectiveFOV);
				float perspectiveNear = camera->GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera->SetPerspectiveNearClip(perspectiveNear);
				float perspectiveFar = camera->GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera->SetPerspectiveFarClip(perspectiveFar);
			}

			else if (camera->GetProjectionType() == Camera::ProjectionType::Orthographic)
			{
				float orthoSize = camera->GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera->SetOrthographicSize(orthoSize);
				float orthoNear = camera->GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera->SetOrthographicNearClip(orthoNear);
				float orthoFar = camera->GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera->SetOrthographicFarClip(orthoFar);
			}
		});

		DrawComponent<StaticModelComponent>("Static Model", entity, [](auto& component)
		{
			ImGui::Button("Drag Model Here...##Static", ImVec2(150.0f, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path modelPath = std::filesystem::path(g_AssetPath) / path;
					std::shared_ptr<StaticModel> model = ResourceManager::GetInstance()->m_ModelLibrary.LoadStatic(modelPath.string());
					if (!model->GetMeshes().empty())
						component.m_StaticModel = model;
					else
						SV_WARN("Could not load model {0}", modelPath.filename().string());
				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::Button("Reset"))
			{
				component.m_StaticModel = std::static_pointer_cast<StaticModel>
					(ResourceManager::GetInstance()->m_ModelLibrary.Get(DEFAULT_STATIC_MODEL));
			}
		});

		DrawComponent<AnimatedModelComponent>("Animated Model", entity, [](auto& component)
		{
			ImGui::Button("Drag Model Here...##Animated", ImVec2(150.0f, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path modelPath = std::filesystem::path(g_AssetPath) / path;
					std::shared_ptr<AnimatedModel> model = ResourceManager::GetInstance()->m_ModelLibrary.LoadAnimated(modelPath.string());
					if (!model->GetMeshes().empty())
						component.m_AnimatedModel = model;
					else
						SV_WARN("Could not load model {0}", modelPath.filename().string());
				}
				ImGui::EndDragDropTarget();
			}

			auto& model = component.m_AnimatedModel;
			auto& animator = component.m_Animator;
			std::vector<std::string> animList = model->GetAnimationList();
			std::string currAnimtaion = animator->GetCurrAnimation() == nullptr ? "" : animator->GetCurrAnimation()->GetCopyName();
			if (ImGui::BeginCombo("Set Animation", currAnimtaion.c_str()))
			{
				for (unsigned int i = 0; i < animList.size(); ++i)
				{
					bool isSelected = currAnimtaion == animList[i];
					if (ImGui::Selectable(animList[i].c_str(), isSelected))
					{
						currAnimtaion = animList[i];
						animator->BindAnimation(model->GetAnimation(animList[i]));
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("Reset"))
			{
				component.m_AnimatedModel = std::static_pointer_cast<AnimatedModel>
					(ResourceManager::GetInstance()->m_ModelLibrary.Get(DEFAULT_ANIMATED_MODEL));
				component.UnbindAnimation();
			}
		});

		DrawComponent<Texture2DComponent>("Texture2D", entity, [](auto& component)
		{
			ImGui::Button("Drag Texture Here...", ImVec2(150.0f, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
					std::shared_ptr<Texture2D> texture = ResourceManager::GetInstance()->m_TextureLibrary.LoadTexture2D(texturePath.string());
					if (texture->GetRendererID() != -1)
						component.m_Texture = texture;
					else
						SV_WARN("Could not load texture {0}", texturePath.filename().string());
				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::Button("Reset"))
			{
				component.m_Texture.reset();
			}
		});

		DrawComponent<ShaderComponent>("Shader", entity, [](auto& component)
		{
			ImGui::Button("Drag Shader Here...", ImVec2(150.0f, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path shaderPath = std::filesystem::path(g_AssetPath) / path;
					std::shared_ptr<Shader> shader = ResourceManager::GetInstance()->m_ShaderLibrary.Load(shaderPath.string());
					if (true)
						component.m_Shader = shader;
					else
						SV_WARN("Could not load shader {0}", shaderPath.filename().string());
				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::Button("Reset"))
			{
				component.m_Shader = ResourceManager::GetInstance()->m_ShaderLibrary.Get(DEFAULT_SHADER);
			}
		});

		//DrawComponent<ScriptComponent>("Native Script", entity, [](auto& component)
		//{
		//	ImGui::Button("Drag Model Here...", ImVec2(150.0f, 0.0f));
		//	if (ImGui::BeginDragDropTarget())
		//	{
		//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		//		{
		//			const wchar_t* path = (const wchar_t*)payload->Data;
		//			std::filesystem::path shaderPath = std::filesystem::path(g_AssetPath) / path;
		//			std::shared_ptr<ScriptableEntity> instance = ResourceManager::GetInstance()->m_ShaderLibrary.Load(shaderPath.string());
		//			if (true)
		//				component.Bind(instance);
		//			else
		//				SV_WARN("Could not load script {0}", shaderPath.filename().string());
		//		}
		//		ImGui::EndDragDropTarget();
		//	}

		//	if (ImGui::Button("Reset"))
		//	{
		//		component.Unbind(instance);
		//	}
		//});
	}

}