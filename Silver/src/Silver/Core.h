#pragma once

#ifdef SV_PLATFORM_WINDOWS
	#ifdef SV_BUILD_DLL
		#define SILVER_API __declspec(dllexport)
	#else
		#define SILVER_API __declspec(dllimport)
	#endif
#else 
	#error Silver only support Windows 
#endif