#include "ImGuiStyles.h"

#include <imgui_internal.h>

namespace Silver {

	void DragFloat3(const std::string& label, glm::vec3& values, glm::vec3& resetValue, float columnWidth)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        if (ImGui::Button("X", buttonSize))
            values.x = resetValue.x;

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.2f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue.y;

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.2f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue.z;

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.2f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

        ImGui::Columns(1);

		ImGui::PopID();
	}

}