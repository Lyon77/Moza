#pragma once

#include "Moza/Core/Buffer.h"
#include "Moza/Renderer/Buffer.h"

namespace Moza
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
		OpenGLVertexBuffer(float * verticies, uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Static);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetData(void* data, uint32_t size, uint32_t offset = 0) override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; };
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Size;
		BufferLayout m_Layout;
		VertexBufferUsage m_Usage;

		Buffer m_LocalData;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t size);
		OpenGLIndexBuffer(uint32_t * indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetData(void* data, uint32_t count, uint32_t offset = 0) override;

		virtual uint32_t GetCount() const override { return m_Count; };
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Count;
		uint32_t m_Size;

		Buffer m_LocalData;
	};
}