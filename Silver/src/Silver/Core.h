#pragma once

#ifdef SV_PLATFORM_WINDOWS
#if SILVER_DLL
	#ifdef SV_BUILD_DLL
		#define SILVER_API __declspec(dllexport)
	#else
		#define SILVER_API __declspec(dllimport)
	#endif
#else
	#define SILVER_API
#endif
#else 
	#error Silver only support Windows 
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn)  std::bind(&fn, this, std::placeholders::_1)

#define SAFE_DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;