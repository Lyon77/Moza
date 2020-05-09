#include "mzpch.h"
#include "OpenGLFramebuffer.h"

#include "Moza/Renderer/Renderer.h"
#include <glad/glad.h>


Moza::OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height, FramebufferFormat format)
	: m_Width(width), m_Height(height), m_Format(format)
{
	Resize(width, height);
}

Moza::OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
	MZ_PROFILE_FUNCTION();

	glDeleteFramebuffers(1, &m_RendererID);
}

void Moza::OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
{
	if (m_Width == width && m_Height == height)
		return;

	m_Width = width;
	m_Height = height;


	if (m_RendererID)
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	glGenFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	glGenTextures(1, &m_ColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

	// TODO: Create texture object based on format here
	if (m_Format == FramebufferFormat::RGBA16F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
	}
	else if (m_Format == FramebufferFormat::RGBA8)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

	glGenTextures(1, &m_DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0,
		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
	);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		MZ_CORE_ERROR("Framebuffer is incomplete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Moza::OpenGLFrameBuffer::Bind() const
{
	MZ_PROFILE_FUNCTION();

	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	glViewport(0, 0, m_Width, m_Height);
}

void Moza::OpenGLFrameBuffer::Unbind() const
{
	MZ_PROFILE_FUNCTION();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Moza::OpenGLFrameBuffer::BindTexture(uint32_t slot) const
{
	MZ_PROFILE_FUNCTION();

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
}