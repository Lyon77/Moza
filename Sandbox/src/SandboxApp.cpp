#include <Moza.h>

class ExampleLayer : public Moza::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		MZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(Moza::Event& event) override
	{
		MZ_TRACE("{0}", event);
	}
};

class Sandbox : public Moza::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};


Moza::Application* Moza::CreateApplication()
{
	return new Sandbox();
}