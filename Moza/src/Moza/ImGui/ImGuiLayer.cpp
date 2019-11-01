#include "mzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "Moza/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Moza
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer") {}


	ImGuiLayer::~ImGuiLayer() {}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		//Temporary
		io.KeyMap[ImGuiKey_Tab] = MZ_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = MZ_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = MZ_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = MZ_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = MZ_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = MZ_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = MZ_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = MZ_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = MZ_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = MZ_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = MZ_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = MZ_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = MZ_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = MZ_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = MZ_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = MZ_KEY_A;
		io.KeyMap[ImGuiKey_C] = MZ_KEY_C;
		io.KeyMap[ImGuiKey_V] = MZ_KEY_V;
		io.KeyMap[ImGuiKey_X] = MZ_KEY_X;
		io.KeyMap[ImGuiKey_Y] = MZ_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = MZ_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());		
	}

	void ImGuiLayer::OnEvent(Event & event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseButtonPressedEvent>(MZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(MZ_BIND_EVENT_FN(ImGuiLayer::OnMouseReleasedPressedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(MZ_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(MZ_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(MZ_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(MZ_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(MZ_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(MZ_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}
	bool ImGuiLayer::OnMouseReleasedPressedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}
	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}
	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.GetYOffset();
		io.MouseWheelH += e.GetXOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetKeyCode()] = true;
		
		io.KeyCtrl = io.KeysDown[MZ_KEY_LEFT_CONTROL] || io.KeysDown[MZ_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[MZ_KEY_LEFT_SHIFT] || io.KeysDown[MZ_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[MZ_KEY_LEFT_ALT] || io.KeysDown[MZ_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[MZ_KEY_LEFT_SUPER] || io.KeysDown[MZ_KEY_RIGHT_SUPER];

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}