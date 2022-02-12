#include "pch.h"
#include "TextureLibrary.h"

namespace Silver {

	void TextureLibrary::Add(const std::shared_ptr<Texture>& texture)
	{
		auto& name = texture->GetName();
		Add(name, texture);
	}

	void TextureLibrary::Add(const std::string& name, const std::shared_ptr<Texture>& texture)
	{
		if (IsExist(name))
			SV_CORE_WARN("texture {0} already exist in TextureLibrary !!!", name);
		m_Textures[name] = texture;
	}

	std::shared_ptr<Texture2D> TextureLibrary::LoadTexture2D(const std::string& filepath)
	{
		auto texture = std::make_shared<Texture2D>(filepath);
		Add(texture);
		return texture;
	}

	std::shared_ptr<Texture2D> TextureLibrary::LoadTexture2D(const std::string& name, const std::string& filepath)
	{
		auto texture = std::make_shared<Texture2D>(filepath);
		Add(name, texture);
		return texture;
	}

	std::shared_ptr<Texture> TextureLibrary::Get(const std::string& name)
	{
		if (!IsExist(name))
			SV_CORE_ERROR("texture {0} does not exist in TextureLibrary !!!", name);
		return m_Textures[name];
	}

	bool TextureLibrary::IsExist(const std::string& name) const
	{
		return m_Textures.find(name) != m_Textures.end();
	}

}