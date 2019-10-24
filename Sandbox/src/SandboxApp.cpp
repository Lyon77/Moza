#include <Moza.h>

class Sandbox : public Moza::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};


Moza::Application* Moza::CreateApplication()
{
	return new Sandbox();
}