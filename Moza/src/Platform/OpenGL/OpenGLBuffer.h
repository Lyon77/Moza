#pragma once

#include "Moza/Renderer/Buffer.h"

namespace Moza
{
	class MOZA_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float * verticies, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;
	private:
		uint32_t m_RendererID;
	};

	class MOZA_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t * indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual uint32_t GetCount() const override { return m_Count; };
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}