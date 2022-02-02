#pragma once

namespace Silver {

	struct FramebufferSpec
	{
		unsigned int Width, Height;
		unsigned int Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpec& spec);
		~Framebuffer();

		void Invalidate();
		void Bind() const;
		void Unbind() const;

		unsigned int GetColorAttachmentRendererID() const { return m_ColorAttachment; }

	private:
		unsigned int m_RendererID;
		unsigned int m_ColorAttachment, m_DepthAttachment;
		FramebufferSpec m_Specification;
	};

}