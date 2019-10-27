#pragma once


#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Moza
{
	class MOZA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core Log Macros
#define MZ_CORE_FATAL(...)  ::Moza::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define MZ_CORE_ERROR(...)  ::Moza::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MZ_CORE_WARN(...)   ::Moza::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MZ_CORE_INFO(...)   ::Moza::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MZ_CORE_TRACE(...)  ::Moza::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client Log Macros
#define MZ_FATAL(...)		::Moza::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define MZ_ERROR(...)		::Moza::Log::GetClientLogger()->error(__VA_ARGS__)
#define MZ_WARN(...)		::Moza::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MZ_INFO(...)		::Moza::Log::GetClientLogger()->info(__VA_ARGS__)
#define MZ_TRACE(...)		::Moza::Log::GetClientLogger()->trace(__VA_ARGS__)