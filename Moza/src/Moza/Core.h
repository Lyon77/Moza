#pragma once

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