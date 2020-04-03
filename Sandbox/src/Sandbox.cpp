#include <Ancora.h>
#include <Ancora/Core/EntryPoint.h>

#include "GameLayer.h"

class Sandbox : public Ancora::Application
{
public:
	Sandbox()
	{
		PushLayer(new GameLayer());
	}

	~Sandbox()
	{
	}

};

Ancora::Application* Ancora::CreateApplication()
{
	return new Sandbox();
}
