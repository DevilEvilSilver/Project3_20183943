#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Silver {

	class Log
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

// Core log macros
#define SV_CORE_TRACE(...)	::Silver::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SV_CORE_INFO(...)	::Silver::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SV_CORE_WARN(...)	::Silver::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SV_CORE_ERROR(...)	::Silver::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SV_CORE_FATAL(...)	::Silver::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define SV_TRACE(...)		::Silver::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SV_INFO(...)		::Silver::Log::GetClientLogger()->info(__VA_ARGS__)
#define SV_WARN(...)		::Silver::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SV_ERROR(...)		::Silver::Log::GetClientLogger()->error(__VA_ARGS__)
#define SV_FATAL(...)		::Silver::Log::GetClientLogger()->fatal(__VA_ARGS__)