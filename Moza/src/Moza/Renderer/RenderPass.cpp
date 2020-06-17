#include "mzpch.h"
#include "Moza/Renderer/RenderPass.h"

#include "Moza/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLRenderPass.h"

namespace Moza
{
	Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    MZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLRenderPass>(spec);
		}

		MZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;

	}
}