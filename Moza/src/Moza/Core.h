#pragma once

#ifdef MZ_PLATFORM_WINDOWS
	#ifdef MZ_BUILD_DLL
		#define MOZA_API __declspec(dllexport)
	#else
		#define MOZA_API __declspec(dllimport)
	#endif
#else
	#error Moza only supports Windows!
#endif