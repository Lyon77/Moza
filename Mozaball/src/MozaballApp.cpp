#include <Moza.h>
#include <Moza/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Moza
{
	class Mozaball : public Application
	{
	public:
		Mozaball()
			: Application("Mozaball")
		{
			PushLayer(new EditorLayer());
		}

		~Mozaball()
		{

		}
	};


	Application* CreateApplication()
	{
		return new Mozaball();
	}
}

