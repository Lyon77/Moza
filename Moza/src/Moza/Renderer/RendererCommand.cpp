#include "mzpch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Moza
{
	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;
}