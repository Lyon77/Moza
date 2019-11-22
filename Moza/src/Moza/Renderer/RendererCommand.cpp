#include "mzpch.h"
#include "Moza/Renderer/RendererCommand.h"

namespace Moza
{
	Scope<RendererAPI> RendererCommand::s_RendererAPI = RendererAPI::Create();
}