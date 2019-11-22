#include "mzpch.h"
#include "Moza/Renderer/VertexArray.h"

#include "Moza/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Moza
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    MZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		MZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}