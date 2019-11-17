#include "mzpch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Moza
{
	Scope<RendererAPI> RendererCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();
}