#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Moza/Events/Event.h"
#include "Moza/Events/ApplicationEvent.h"

#include "Moza/ImGui/ImGuiLayer.h"

#include "Moza/Renderer/Shader.h"
#include "Moza/Renderer/Buffer.h"

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

		unsigned int m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		std::unique_ptr<Shader> m_Shader;
	private:
		static Application* s_Instance;
	};

	//To be defined in client
	Application* CreateApplication();
}

