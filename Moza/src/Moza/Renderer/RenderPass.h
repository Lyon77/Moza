#pragma once

#include "Moza/Core/Core.h"

#include "Moza/Renderer/Framebuffer.h"

namespace Moza
{
	struct RenderPassSpecification
	{
		Ref<Framebuffer> TargetFramebuffer;
	};

	class RenderPass
	{
	public:
		virtual ~RenderPass() {}

		virtual const RenderPassSpecification& GetSpecification() const = 0;

		static Ref<RenderPass> Create(const RenderPassSpecification& spec);
	};
}