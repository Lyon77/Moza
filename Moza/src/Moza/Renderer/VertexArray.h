#pragma once

#include "Buffer.h"


namespace Moza
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() = 0;

		static Ref<VertexArray> Create();
	};
}