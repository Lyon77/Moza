#pragma once

#include <glm/glm.hpp>

#include "Moza/Renderer/RendererAPI.h"

namespace Moza
{
	enum class FramebufferFormat
	{
		None = 0,
		RGBA8 = 1,
		RGBA16F = 2
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 1280;
		uint32_t Height = 720;
		glm::vec4 ClearColor;
		FramebufferFormat Format;

		// SwapChainTarget = screen buffer (i.e. no framebuffer)
		bool SwapChainTarget = false;

	};

	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

		virtual ~Framebuffer() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void BindTexture(uint32_t slot = 0) const = 0;

		virtual uint32_t GetRendererID() const = 0;
		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual uint32_t GetDepthAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
	};

	class FramebufferPool final
	{
	public:
		FramebufferPool(uint32_t maxFBs = 32);
		~FramebufferPool();

		std::weak_ptr<Framebuffer> AllocateBuffer();
		void Add(std::weak_ptr<Framebuffer> framebuffer);

		const std::vector<std::weak_ptr<Framebuffer>>& GetAll() const { return m_Pool; }

		inline static Ref<FramebufferPool> GetGlobal() { return s_Instance; }
	private:
		std::vector<std::weak_ptr<Framebuffer>> m_Pool;
		static Ref<FramebufferPool> s_Instance;
	};
}