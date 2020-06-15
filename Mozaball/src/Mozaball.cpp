#include <Moza.h>
#include <Moza/Core/EntryPoint.h>

#include "EditorLayer.h"

class Mozaball : public Moza::Application
{
public:
	Mozaball(const Moza::ApplicationProps& props)
		: Application(props)
	{
		PushLayer(new Moza::EditorLayer());
	}

	~Mozaball()
	{
		
	}
};


Moza::Application* Moza::CreateApplication()
{
	return new Mozaball({"Mozaball", 1600, 900});
}