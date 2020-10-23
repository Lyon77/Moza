#pragma once


#include "Moza/Core/Core.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

namespace Moza
{
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

//Core Log Macros
#define MZ_CORE_CRITICAL(...)  ::Moza::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define MZ_CORE_ERROR(...)  ::Moza::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MZ_CORE_WARN(...)   ::Moza::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MZ_CORE_INFO(...)   ::Moza::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MZ_CORE_TRACE(...)  ::Moza::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client Log Macros
#define MZ_CRITICAL(...)		::Moza::Log::GetClientLogger()->critical(__VA_ARGS__)
#define MZ_ERROR(...)		::Moza::Log::GetClientLogger()->error(__VA_ARGS__)
#define MZ_WARN(...)		::Moza::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MZ_INFO(...)		::Moza::Log::GetClientLogger()->info(__VA_ARGS__)
#define MZ_TRACE(...)		::Moza::Log::GetClientLogger()->trace(__VA_ARGS__)