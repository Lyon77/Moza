#pragma once
#include "Moza/Renderer/Framebuffer.h"

namespace Moza
{
	class OpenGLFrameBuffer : public Framebuffer
	{
	public:
		OpenGLFrameBuffer(uint32_t width, uint32_t height, FramebufferFormat format);
		virtual ~OpenGLFrameBuffer();

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void BindTexture(uint32_t slot = 0) const override;

		virtual uint32_t GetRendererID() const { return m_RendererID; }
		virtual uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }
		virtual uint32_t GetDepthAttachmentRendererID() const { return m_DepthAttachment; }

		virtual uint32_t GetWidth() const { return m_Width; }
		virtual uint32_t GetHeight() const { return m_Height; }
		virtual FramebufferFormat GetFormat() const { return m_Format; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Width, m_Height;
		FramebufferFormat m_Format;

		uint32_t m_ColorAttachment, m_DepthAttachment;

	};
}