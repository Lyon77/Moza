#pragma once

#ifdef MZ_DEBUG
	#define MZ_ENABLE_ASSERTS
#endif


#ifdef MZ_ENABLE_ASSERTS
	#define MZ_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { MZ_ERROR("Assertion Failed"); __debugbreak(); } }
	#define MZ_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { MZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

	#define MZ_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro
	#define MZ_GET_ASSERT_MACRO(...) MZ_EXPAND_VARGS(MZ_ASSERT_RESOLVE(__VA_ARGS__, MZ_ASSERT_MESSAGE, MZ_ASSERT_NO_MESSAGE))

	#define MZ_ASSERT(...) MZ_EXPAND_VARGS( MZ_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
	#define MZ_CORE_ASSERT(...) MZ_EXPAND_VARGS( MZ_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
	#define MZ_ASSERT(...)
	#define MZ_CORE_ASSERT(...)
#endif
