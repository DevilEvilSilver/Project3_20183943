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
		void Resize(unsigned int width, unsigned int height); // Creating new rendering id when resize !!!
		void Bind() const;
		void Unbind() const;

		const FramebufferSpec& GetSpecification() const { return m_Specification; }
		unsigned int GetColorAttachmentRendererID() const { return m_ColorAttachment; }

	private:
		unsigned int m_RendererID = 0;
		unsigned int m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpec m_Specification;
	};

}