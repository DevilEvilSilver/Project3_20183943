#include "ResourcePanel.h"
#include "DataManager/Resources/ResourceManager.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Silver {


	ResourcePanel::ResourcePanel()
	{
	}

	template <typename T>
	void DrawResourceNode(T& res)
	{
		auto& tag = res.GetName();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)res, flags, tag.c_str());

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete")) {}
				

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void ResourcePanel::OnImGuiRender()
	{
		ImGui::Begin("Resource Manager");

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_AllowItemOverlap;

		// Model Library
		{
			bool opened = ImGui::TreeNodeEx((void*)typeid(Model).hash_code(), treeNodeFlags, "Model Library");
			if (opened)
			{

				for (auto& model : ResourceManager::GetInstance()->m_ModelLibrary.GetLibrary())
				{
					auto& tag = model.second->GetName();

					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
					flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
					bool opened = ImGui::TreeNodeEx((void*)(uint64_t)tag.c_str(), flags, tag.c_str());

					bool deleted = false;
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Delete"))
							deleted = true;

						ImGui::EndPopup();
					}

					if (opened)
					{
						ImGui::TreePop();
					}

					if (deleted)
					{
						ResourceManager::GetInstance()->m_ModelLibrary.Remove(model.first);
						break;
					}
				}
				ImGui::TreePop();
			}
		}

		// Texture Library
		{
			bool opened = ImGui::TreeNodeEx((void*)typeid(Texture2D).hash_code(), treeNodeFlags, "Texture Library");
			if (opened)
			{

				for (auto& texture : ResourceManager::GetInstance()->m_TextureLibrary.GetLibrary())
				{
					auto& tag = texture.second->GetName();

					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
					flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
					bool opened = ImGui::TreeNodeEx((void*)(uint64_t)tag.c_str(), flags, tag.c_str());

					bool deleted = false;
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Delete"))
							deleted = true;

						ImGui::EndPopup();
					}

					if (opened)
					{
						ImGui::TreePop();
					}

					if (deleted)
					{
						ResourceManager::GetInstance()->m_TextureLibrary.Remove(texture.first);
						break;
					}
				}
				ImGui::TreePop();
			}
		}

		// Shader Library
		{
			bool opened = ImGui::TreeNodeEx((void*)typeid(Shader).hash_code(), treeNodeFlags, "Shader Library");
			if (opened)
			{

				for (auto& shader : ResourceManager::GetInstance()->m_ShaderLibrary.GetLibrary())
				{
					auto& tag = shader.second->GetName();

					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
					flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
					bool opened = ImGui::TreeNodeEx((void*)(uint64_t)tag.c_str(), flags, tag.c_str());

					bool deleted = false;
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Delete"))
							deleted = true;

						ImGui::EndPopup();
					}

					if (opened)
					{
						ImGui::TreePop();
					}

					if (deleted)
					{
						ResourceManager::GetInstance()->m_ShaderLibrary.Remove(shader.first);
						break;
					}
				}
				ImGui::TreePop();
			}
		}

		ImGui::End();
	}

}