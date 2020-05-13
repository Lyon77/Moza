#pragma once

#include "Moza/Core/Core.h"
#include "Moza/Core/Buffer.h"

#include <string>

namespace Moza
{
	enum class TextureFormat
	{
		None = 0,
		RGB = 1,
		RGBA = 2,
	};

	enum class TextureWrap
	{
		None = 0,
		Clamp = 1,
		Repreat = 2
	};

	class Texture
	{
	public:
		static uint32_t GetBPP(TextureFormat format);

		virtual TextureFormat GetFormat() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;		

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual const std::string& GetPath() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(TextureFormat format, uint32_t width, uint32_t height, TextureWrap wrap = TextureWrap::Clamp);
		static Ref<Texture2D> Create(const std::string& path, bool srgb = false);

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Lock() = 0;
		virtual void Unlock() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual Buffer GetWriteableBuffer() = 0;
	};

	class TextureCube : public Texture
	{
	public:
		static Ref<TextureCube> Create(const std::string& path);
		static Ref<TextureCube> Create(const std::vector<std::string> textures_faces);
	};
}