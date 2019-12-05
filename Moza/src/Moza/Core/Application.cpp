#include "mzpch.h"
#include "Moza/Core/Application.h"

#include "Moza/Core/Log.h"

#include "Moza/Renderer/Renderer.h"

#include "Moza/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Moza
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		MZ_PROFILE_FUNCTION();

		MZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = Window::Create();
		m_Window->SetEventCallback(MZ_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		MZ_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}


	void Application::PushLayer(Layer * layer)
	{
		MZ_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer * overlay)
	{
		MZ_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		MZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(MZ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(MZ_BIND_EVENT_FN(Application::OnWindowResize));

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
		MZ_PROFILE_FUNCTION();

		while (m_Running)
		{
			MZ_PROFILE_SCOPE("RunLoop");

			float time = (float) glfwGetTime(); // Platform::GetTime() in the future
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				MZ_PROFILE_SCOPE("LayerStack OnUpdate");

				//update each layer from begining to end
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			{
				MZ_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		MZ_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}
