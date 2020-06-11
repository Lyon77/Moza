#pragma once

#include "Moza/Core/Core.h"
#include "Moza/Core/Log.h"

namespace Moza {

	struct Buffer
	{
		unsigned char* Data;
		uint32_t Size;

		Buffer()
			: Data(nullptr), Size(0)
		{
		}

		Buffer(unsigned char* data, uint32_t size)
			: Data(data), Size(size)
		{
		}

		void Allocate(uint32_t size)
		{
			delete[] Data;
			Data = nullptr;

			if (size == 0)
				return;

			Data = new unsigned char[size];
			Size = size;
		}

		void ZeroInitialize()
		{
			if (Data)
				memset(Data, 0, Size);
		}

		void Write(unsigned char* data, uint32_t size, uint32_t offset = 0)
		{
			MZ_CORE_ASSERT(offset + size <= Size, "Buffer overflow!");
			memcpy(Data + offset, data, size);
		}

		operator bool() const
		{
			return Data;
		}

		unsigned char& operator[](int index)
		{
			return Data[index];
		}

		unsigned char operator[](int index) const
		{
			return Data[index];
		}

		template<typename T>
		T* As()
		{
			return (T*)Data;
		}

		inline uint32_t GetSize() const { return Size; }
	};

}