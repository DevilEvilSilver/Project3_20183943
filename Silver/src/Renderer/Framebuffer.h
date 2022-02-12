#pragma once

#include <vector>

namespace Silver {

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/ Stencil
		DEPTH24_STENCIL8,

		// Defaults
		Depth = DEPTH24_STENCIL8
	};

	struct FramebufferTextureSpec
	{
		FramebufferTextureSpec() = default;
		FramebufferTextureSpec(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpec
	{
		FramebufferAttachmentSpec() = default;
		FramebufferAttachmentSpec(const std::initializer_list<FramebufferTextureSpec>& list)
			: Attachments(list) {}

		std::vector<FramebufferTextureSpec> Attachments;
	};

	struct FramebufferSpec
	{
		unsigned int Width, Height;
		FramebufferAttachmentSpec Attachments;
		unsigned int Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpec& spec);
		~Framebuffer();

		void Invalidate();
		void Resize(unsigned int width, unsigned int height); // Creating new rendering id when resize !!!
		void Bind() const;
		void Unbind() const;

		int ReadPixel(unsigned int attachmentIndex, int x, int y);
		void ClearAttachment(unsigned int attachmentIndex, int value);
		const FramebufferSpec& GetSpecification() const { return m_Specification; }
		unsigned int GetColorAttachmentRendererID(unsigned int index = 0) const 
		{ 
			if (index < m_ColorAttachments.size())
				return m_ColorAttachments[index];
			SV_CORE_ERROR("Get Color Attachment out of range !!!");
			return 0;
		}

	private:
		unsigned int m_RendererID = 0;
		//unsigned int m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpec m_Specification;

		std::vector<FramebufferTextureSpec> m_ColorAttachmentSpecs;
		FramebufferTextureSpec m_DepthAttachmentSpec;

		std::vector<unsigned int> m_ColorAttachments;
		unsigned int m_DepthAttachment = 0;
	};

}