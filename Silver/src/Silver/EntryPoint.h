#pragma once

#ifdef SV_PLATFORM_WINDOWS

extern Silver::Application* Silver::CreateApplication();

void main(int argc, char** argv)
{
	Silver::Log::Init();
	SV_INFO("Start up ! ! !");

	auto app = Silver::CreateApplication();
	app->Run();
	delete app;
}

#else 
	#error Silver only support Windows 
#endif