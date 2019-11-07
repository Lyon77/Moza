#include "mzpch.h"
#include "Application.h"

#include "Log.h"

#include "Moza/Renderer/Renderer.h"

#include "Input.h"

#include <GLFW/glfw3.h>

namespace Moza
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		MZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}


	void Application::PushLayer(Layer * layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer * overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		//MZ_CORE_TRACE("{0}", e);

		//events are triggered from the last layer to the first
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			//REWORD this line
			(*--it)->OnEvent(e);

			//if a layer interacts with event, make sure other layers don't interact with it
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float) glfwGetTime(); // Platform::GetTime() in the future
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			//update each layer from begining to end
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);
			
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}
