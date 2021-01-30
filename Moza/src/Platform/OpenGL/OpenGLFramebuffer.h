#pragma once

#include "Moza/Renderer/Framebuffer.h"

namespace Moza
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentSize, int x, int y) override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { MZ_CORE_ASSERT(index < m_ColorAttachments.size(), "Index out of bounds!");  return m_ColorAttachments[index]; };

		virtual const FramebufferSpecification& GetSpecification() const override {	return m_Specification;	};
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}
