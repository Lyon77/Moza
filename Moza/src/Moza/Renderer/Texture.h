#pragma once

#include "Moza/Core.h"
#include <string>

namespace Moza
{
	class MOZA_API Texture
	{
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class MOZA_API Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);

	};
}