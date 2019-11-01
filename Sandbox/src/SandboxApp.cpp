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
		if (Moza::Input::IsKeyPressed(MZ_KEY_TAB))
			MZ_TRACE("Tab key has been pressed");
	}

	void OnEvent(Moza::Event& event) override
	{
		if (event.GetEventType() == Moza::EventType::KeyPressed)
		{
			Moza::KeyPressedEvent& e = (Moza::KeyPressedEvent&)event;
			if(e.GetKeyCode() == MZ_KEY_TAB)
				MZ_TRACE("Tab key has been pressed");
			MZ_TRACE("{0}", (char)e.GetKeyCode());
		}
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