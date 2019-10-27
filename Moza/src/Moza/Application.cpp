#include "mzpch.h"
#include "Application.h"

#include "Log.h"
#include "Moza/Events/ApplicationEvent.h"

namespace Moza
{
	Application::Application()
	{

	}


	Application::~Application()
	{
	}

	void Application::Run()
	{
		//Test
		WindowResizeEvent e(1200, 720);
		if (e.IsInCategory(EventCategoryApplication)) 
		{
			MZ_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			MZ_TRACE(e);
		}

		while (true);
	}

}
