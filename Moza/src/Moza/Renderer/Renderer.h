#pragma once

namespace Moza
{
	enum class MOZA_API RendererAPI
	{
		None = 0, OpenGL = 1
	};

	class MOZA_API Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};
}