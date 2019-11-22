#include "mzpch.h"
#include "Moza/Renderer/GraphicsContext.h"

#include "Moza/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Moza
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:   MZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		MZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}