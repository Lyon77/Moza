#include "mzpch.h"
#include "Moza/Core/Application.h"

#include "Moza/Core/Log.h"

#include "Moza/Renderer/Renderer.h"
#include "Moza/Renderer/Framebuffer.h"

#include "Moza/Core/Input.h"

#include <imgui.h>

#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <commdlg.h>

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

	void Application::RenderImGui()
	{
		MZ_PROFILE_SCOPE("LayerStack OnImGuiRender");

		m_ImGuiLayer->Begin();

		
		ImGui::Begin("Renderer");
		std::string vendor = (const char*)glGetString(GL_VENDOR);
		std::string renderer = (const char*)glGetString(GL_RENDERER);
		std::string version = (const char*)glGetString(GL_VERSION);
		ImGui::Text("Vendor: %s", vendor.c_str());
		ImGui::Text("Renderer: %s", renderer.c_str());
		ImGui::Text("Version: %s", version.c_str());
		ImGui::End();

		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();

		m_ImGuiLayer->End();
	}

	std::string Application::OpenFile(const std::string& filter) const
	{
		OPENFILENAMEA ofn;

		CHAR szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)m_Window->GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "All\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
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

			RenderImGui();

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
