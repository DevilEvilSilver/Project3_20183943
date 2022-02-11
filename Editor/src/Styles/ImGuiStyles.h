#pragma once

#include <string>
#include <imgui.h>
#include <glm/glm.hpp>

namespace Silver {

	void DragFloat3(const std::string& label, glm::vec3& values, glm::vec3& resetValue = glm::vec3(0.0f), float columnWidth = 100.0f);

}