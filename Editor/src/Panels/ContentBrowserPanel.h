#pragma once
#include "Renderer/Texture.h"

#include <memory>
#include <filesystem>

namespace Silver {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrDirectory;

		std::shared_ptr<Texture2D> m_DirectoryIcon;
		std::shared_ptr<Texture2D> m_FileIcon;
	};

}