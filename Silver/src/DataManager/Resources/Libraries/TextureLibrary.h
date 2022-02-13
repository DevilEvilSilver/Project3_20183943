#pragma once
#include "Renderer/Texture.h"

#include <string>
#include <unordered_map>
#include <memory>

namespace Silver {

	class TextureLibrary
	{
	public:
		void Add(const std::shared_ptr<Texture>& texture);
		void Add(const std::string& name, const std::shared_ptr<Texture>& texture);
		void Remove(const std::string& name);
		std::shared_ptr<Texture2D> LoadTexture2D(const std::string& filepath);
		std::shared_ptr<Texture2D> LoadTexture2D(const std::string& name, const std::string& filepath);

		std::shared_ptr<Texture> Get(const std::string& name);
		const std::unordered_map<std::string, std::shared_ptr<Texture>>& GetLibrary() { return m_Textures; }

		bool IsExist(const std::string& name) const;

	private:
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
	};

}