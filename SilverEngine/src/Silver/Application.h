#pragma once
#include "Core.h"

namespace Silver {

	class SILVER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//To be define in applications
	Application* CreateApplication();

}
