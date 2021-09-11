#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Silver {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log:: s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("SILVER_ENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("GAME");
		s_CoreLogger->set_level(spdlog::level::trace);
	}

}