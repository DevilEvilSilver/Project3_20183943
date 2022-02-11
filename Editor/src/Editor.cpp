#include <Silver.h>

#include "EditorLayer.h"

namespace Silver {

	class Editor : public Application
	{
	public:
		Editor()
			:Application("Silver Editor", 1600, 900)
		{
			PushLayer(new EditorLayer());
		}

		~Editor()
		{

		}

	};

	Application* CreateApplication()
	{
		return new Editor();
	}

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