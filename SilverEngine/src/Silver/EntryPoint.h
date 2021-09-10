#pragma once

#ifdef SV_PLATFORM_WINDOWS

extern Silver::Application* Silver::CreateApplication();

void main(int argc, char** argv)
{
	auto app = Silver::CreateApplication();
	app->Run();
	delete app;
}

#else 
	#error Silver only support Windows 
#endif