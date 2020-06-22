#pragma once

#include "Moza/Renderer/Texture.h"

#include <glad/glad.h>

namespace Moza
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(TextureFormat format, uint32_t width, uint32_t height, TextureWrap wrap);
		OpenGLTexture2D(const std::string& path, bool srgb);
		virtual ~OpenGLTexture2D();

		virtual TextureFormat GetFormat() const { return m_Format; }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual const std::string& GetPath() const override { return m_Path; }
		virtual uint32_t GetMipLevelCount() const override;
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual bool Loaded() const override { return m_Loaded; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual void Lock() override;
		virtual void Unlock() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual Buffer GetWriteableBuffer() override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		};
	private:
		std::string m_Path;
		TextureFormat m_Format;
		TextureWrap m_Wrap;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		Buffer m_ImageData;
		bool m_IsHDR = false;
		bool m_Locked = false;
		bool m_Loaded = false;
	};

	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(TextureFormat format, uint32_t width, uint32_t height);
		OpenGLTextureCube(const std::string& path);
		OpenGLTextureCube(const std::vector<std::string> textures_faces);
		virtual ~OpenGLTextureCube();

		virtual TextureFormat GetFormat() const { return m_Format; }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual const std::string& GetPath() const override { return m_Path; }
		virtual uint32_t GetMipLevelCount() const override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTextureCube&)other).m_RendererID;
		};
	private:
		std::string m_Path;
		TextureFormat m_Format;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		unsigned char* m_ImageData;
	};
}