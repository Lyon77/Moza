#include "mzpch.h"
#include "Moza/Renderer/Framebuffer.h"

#include "Moza/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Moza
{
	Ref<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height, FramebufferFormat format)
	{
		Moza::Framebuffer* result = nullptr;

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    MZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFrameBuffer>(width, height, format);
		}
	}

	Ref<FramebufferPool> FramebufferPool::s_Instance = CreateRef<FramebufferPool>();

	FramebufferPool::FramebufferPool(uint32_t maxFBs)
	{
	}

	FramebufferPool::~FramebufferPool()
	{
	}

	std::weak_ptr<Framebuffer> FramebufferPool::AllocateBuffer()
	{
		return std::weak_ptr<Framebuffer>();
	}

	void FramebufferPool::Add(Framebuffer* framebuffer)
	{
		m_Pool.push_back(framebuffer);
	}
}