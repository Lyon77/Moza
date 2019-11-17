#pragma once

#include <memory>

#ifdef MZ_PLATFORM_WINDOWS
#if MZ_DYNAMIC_LINK
	#ifdef MZ_BUILD_DLL
		#define MOZA_API __declspec(dllexport)
	#else
		#define MOZA_API __declspec(dllimport)
	#endif
#else
	#define MOZA_API
#endif
#else
	#error Moza only supports Windows!
#endif

#ifdef MZ_DEBUG
	#define MZ_ENABLE_ASSERTS
#endif

#ifdef MZ_ENABLE_ASSERTS
	#define MZ_ASSERT(x, ...) { if(!(x)) {MZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MZ_CORE_ASSERT(x, ...) { if(!(x)) {MZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MZ_ASSERT(x, ...)
	#define MZ_CORE_ASSERT(x, ...)
#endif // MZ_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define MZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Moza
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}