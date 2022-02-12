#pragma once

#include <string>

namespace Silver {

	class Texture
	{
	public:
		Texture(const std::string& filepath);
		virtual ~Texture() = default;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual unsigned int GetRendererID() const = 0;
		virtual const std::string& GetDirectory() const { return m_Directory; }

		virtual void Bind(unsigned int slot = 0) const = 0;

		const std::string& GetName() { return m_Name; }
	protected:
		std::string m_Name;
		std::string m_Directory;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const std::string& path);
		~Texture2D();

		unsigned int GetWidth() const override { return m_Width; }
		unsigned int GetHeight() const override { return m_Height; }
		unsigned int GetRendererID() const override { return m_RendererID; }

		void Bind(unsigned int slot = 0) const override;

	private:
		unsigned int m_RendererID = -1;
		unsigned int m_Width, m_Height;
	};

}