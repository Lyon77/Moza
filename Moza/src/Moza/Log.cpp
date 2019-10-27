#include "mzpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Moza
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		//set the pattern, color -> type -> message
		spdlog::set_pattern("%^[%T] %n: %v%$");

		//set the core logger
		s_CoreLogger = spdlog::stdout_color_mt("MOZA");
		s_CoreLogger->set_level(spdlog::level::trace);

		//set the client logger
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);;
	}

}