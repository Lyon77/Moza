#include "mzpch.h"
#include "Moza/Renderer/Framebuffer.h"

#include "Moza/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Moza
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		Ref<Framebuffer> result = nullptr;

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    MZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFrameBuffer>(spec);
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

	void FramebufferPool::Add(std::weak_ptr<Framebuffer> framebuffer)
	{
		m_Pool.push_back(framebuffer);
	}
}