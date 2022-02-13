#pragma once

#include <memory>
#include <filesystem>

namespace Silver {

	class ResourcePanel
	{
	public:
		ResourcePanel();

		void OnImGuiRender();
	};

}