#pragma once

#include "Core.h"
#include "Moza/Events/Event.h"
#include "Moza/Events/ApplicationEvent.h"

#include "Window.h"

namespace Moza
{ 
	class MOZA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	//To be defined in client
	Application* CreateApplication();
}

