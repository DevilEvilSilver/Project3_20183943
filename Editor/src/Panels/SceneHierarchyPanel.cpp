#include "SceneHierarchyPanel.h"
#include "DataManager/ECS/Components.h"
#include "../Styles/ImGuiStyles.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Silver {

	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& scene)
		:m_Context(scene)
	{
	}

	void SceneHierarchyPanel::SetContext(const std::shared_ptr<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = { entt::null, m_Context.get() };
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entity)
		{
			Entity e { entity, m_Context.get() };
			DrawEntityNode(e);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = { entt::null, m_Context.get() };

		// right click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

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

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DragFloat3("Position", component.Translation, glm::vec3(0.0f), 70.0f);
			DragFloat3("Rotation", component.Rotation, glm::vec3(-90.0f, 0.0f, 0.0f), 70.f);
			DragFloat3("Scale", component.Scale, glm::vec3(1.0f), 70.0f);
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
	}

}