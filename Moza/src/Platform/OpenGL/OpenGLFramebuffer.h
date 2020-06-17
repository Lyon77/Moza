#pragma once
#include "Moza/Renderer/Framebuffer.h"

namespace Moza
{
	class OpenGLFrameBuffer : public Framebuffer
	{
	public:
		OpenGLFrameBuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void BindTexture(uint32_t slot = 0) const override;

		virtual uint32_t GetRendererID() const { return m_RendererID; }
		virtual uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }
		virtual uint32_t GetDepthAttachmentRendererID() const { return m_DepthAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		uint32_t m_ColorAttachment, m_DepthAttachment;

	};
}