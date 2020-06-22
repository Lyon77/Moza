#include "mzpch.h"
#include "Moza/Renderer/Texture.h"

#include "Moza/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Moza
{
	Ref<Texture2D> Texture2D::Create(TextureFormat format, uint32_t width, uint32_t height, TextureWrap wrap)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    MZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(format, width, height, wrap);
		}

		MZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string & path, bool srgb)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    MZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path, srgb);
		}

		MZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<TextureCube> TextureCube::Create(TextureFormat format, uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    MZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTextureCube>(format, width, height);
		}

		MZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<TextureCube> TextureCube::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    MZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTextureCube>(path);
		}

		MZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<TextureCube> TextureCube::Create(const std::vector<std::string> textures_faces)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    MZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTextureCube>(textures_faces);
		}

		MZ_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	uint32_t Texture::GetBPP(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RGB: return 3;
			case TextureFormat::RGBA: return 4;
		}
		return 0;
	}

	uint32_t Texture::CalculateMipMapCount(uint32_t width, uint32_t height)
	{
		uint32_t levels = 1;
		while ((width | height) >> levels)
			levels++;

		return levels;
	}
}