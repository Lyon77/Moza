#pragma once

#include "Moza/Core/Core.h"

#include "Moza/Core/Window.h"
#include "Moza/Core/LayerStack.h"
#include "Moza/Events/Event.h"
#include "Moza/Events/ApplicationEvent.h"

#include "Moza/Core/Timestep.h"

#include "Moza/ImGui/ImGuiLayer.h"

namespace Moza
{ 
	struct ApplicationProps
	{
		std::string Name;
		uint32_t WindowWidth, WindowHeight;
	};

	class Application
	{
	public:
		Application(const ApplicationProps& props = { "Hazel Engine", 1280, 720 });
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void RenderImGui();

		std::string OpenFile(const std::string& filter) const;

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		Timestep m_TimeStep;

		static Application* s_Instance;
	};

	//To be defined in client
	Application* CreateApplication();
}

