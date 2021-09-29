#include <conio.h>
#include <Silver.h>

class TestLayer : public Silver::Layer
{
public:
	TestLayer()
		:Layer("TestLayer")
	{
	}

	void OnUpdate() override
	{
		if (Silver::Input::IsKeyPressed(KEY_A))
			SV_INFO("bruh .....");
	}

	void OnEvent(Silver::Event& event) override
	{
		//SV_TRACE("{0}", event);
	}
};

class Game : public Silver::Application
{
public:
	Game()
	{
		PushLayer(new TestLayer());
		PushOverlay(new Silver::ImGuiLayer());
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