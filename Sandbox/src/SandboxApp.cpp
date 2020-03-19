#include <Moza.h>
#include <Moza/Core/EntryPoint.h>

#include "ExampleLayer.h"
#include "Sandbox2D.h"
#include "GameLayer.h"

class Sandbox : public Moza::Application
{
public:
	Sandbox()
	{
		//PushLayer(new Sandbox2D());
		PushLayer(new GameLayer());
	}

	~Sandbox()
	{
		
	}
};


Moza::Application* Moza::CreateApplication()
{
	return new Sandbox();
}