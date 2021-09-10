#include <conio.h>
#include <Silver.h>

class GameApp : public Silver::Application
{
public:
	GameApp()
	{

	}

	~GameApp()
	{

	}

};

Silver::Application* Silver::CreateApplication()
{
	return new GameApp;
}