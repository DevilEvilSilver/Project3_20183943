#include <conio.h>
#include <Silver.h>

class Game : public Silver::Application
{
public:
	Game()
	{

	}

	~Game()
	{

	}

};

Silver::Application* Silver::CreateApplication()
{
	return new Game;
}

#ifdef SV_PLATFORM_WINDOWS

int main(int argc, char** argv)
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