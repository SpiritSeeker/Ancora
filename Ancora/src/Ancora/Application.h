#pragma once

// #include "Core.h"

namespace  Ancora {
	class __attribute__((visibility("default"))) Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
	};

	Application* CreateApplication();

}