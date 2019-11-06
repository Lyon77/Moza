#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Moza/Events/Event.h"
#include "Moza/Events/ApplicationEvent.h"

#include "Moza/ImGui/ImGuiLayer.h"

#include "Moza/Renderer/Shader.h"
#include "Moza/Renderer/Buffer.h"
#include "Moza/Renderer/VertexArray.h"
#include "Moza/Renderer/OrthographicCamera.h"

namespace Moza
{ 
	class MOZA_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Shader> m_Shader;

		OrthographicCamera m_Camera;
	private:
		static Application* s_Instance;
	};

	//To be defined in client
	Application* CreateApplication();
}

