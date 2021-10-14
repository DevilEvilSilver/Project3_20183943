#include "pch.h"
#include "Texture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Silver {

	Texture2D::Texture2D(const std::string & path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (!data)
		{
			SV_CORE_ERROR("Failed to load image!");
		}
		else {
			m_Width = width;
			m_Height = height;

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, (channels == 3) ? GL_RGB8 : GL_RGBA8, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, (channels == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void Texture2D::Bind(unsigned int slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}