#include "mzpch.h"
#include "Application.h"

#include "Log.h"
#include "Moza/Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>

namespace Moza
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}


	Application::~Application()
	{
	}

	void Application::Run()
	{
		////Test
		//WindowResizeEvent e(1200, 720);
		//if (e.IsInCategory(EventCategoryApplication)) 
		//{
		//	MZ_TRACE(e);
		//}
		//if (e.IsInCategory(EventCategoryInput))
		//{
		//	MZ_TRACE(e);
		//}

		while (true)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

}
