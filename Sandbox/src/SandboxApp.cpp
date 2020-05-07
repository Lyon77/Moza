#include <Moza.h>
#include <Moza/Core/EntryPoint.h>

#include "ExampleLayer.h"
#include "Sandbox2D.h"
#include "ParticleLayer.h"

class Sandbox : public Moza::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
		//PushLayer(new ParticleLayer());
	}

	~Sandbox()
	{
		
	}
};


Moza::Application* Moza::CreateApplication()
{
	return new Sandbox();
}