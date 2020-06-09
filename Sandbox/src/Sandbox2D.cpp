#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles = 
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWDDDDDDWWWWWWWWWWWW"
"WWWWWDDDDDDDDDDDWWWWWWWW"
"WWWWDDDWWWWWDDDDDDDDWWWW"
"WWWWDDDWWWWWDDDDDDDDWWWW"
"WWWWDDDWWWWWDDDDDDDDWWWW"
"WWWWDDDDDDDDDDDDDDDDWWWW"
"WWWWDDDDDDDDDDDWWDDDWWWW"
"WWWWDDDDDDDDDDDWWDDDWWWW"
"WWWWDDDDDDDDDDDDDDDDWWWW"
"WWWWDDDDDDDDDDDDDDDDWWWW"
"WWWWDDDDDDDDDDDDDDDDWWWW"
"WWWWWDDXXDDDDDDDDDWWWWWW"
"WWWWWWDDDDDDDDDWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	MZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Moza::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteSheet = Moza::Texture2D::Create("assets/textures/farm.png");

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	s_TextureMap['D'] = Moza::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 });
	s_TextureMap['W'] = Moza::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11, 11 }, { 128, 128 });
	m_Stairs = Moza::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 1 }, { 128, 128 });

	//m_CameraController.SetZoomLevel(5.0f);

	Moza::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Moza::Framebuffer::Create(fbSpec);
}

void Sandbox2D::OnDetach()
{
	MZ_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Moza::Timestep ts)
{
	MZ_PROFILE_FUNCTION();

	// update Camera
	{
		MZ_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// update Render
	{
		MZ_PROFILE_SCOPE("RendererPrep");
		m_Framebuffer->Bind();

		Moza::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Moza::RendererCommand::Clear();
	}

	{
		MZ_PROFILE_SCOPE("RendererDraw");
		static float angle = 0.0f;
		angle += ts * 20.0f;


		Moza::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Moza::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		Moza::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Moza::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, { 0.2f, 0.8f, 0.3f, 0.75f }, 2.0f);
		Moza::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f,  0.1f }, { 1.0f, 1.0f }, glm::radians(angle), m_CheckerboardTexture, { 0.7f, 0.2f, 0.3f, 0.75f }, 1.0f);

		/*
		// Drawing the Sprite Map
		for (uint32_t y = 0; y < m_MapHeight; y++)
		{
			for (uint32_t x = 0; x < m_MapWidth; x++)
			{
				char tileType = s_MapTiles[x + y * m_MapWidth];
				Moza::Ref<Moza::SubTexture2D> texture;

				if (s_TextureMap.find(tileType) != s_TextureMap.end())
					texture = s_TextureMap[tileType];
				else
					texture = m_Stairs;

				Moza::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f }, { 1.0f, 1.0f }, texture, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
			}
		}
		*/

		Moza::Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}
}

void Sandbox2D::OnImGuiRender()
{
	MZ_PROFILE_FUNCTION();

	static bool dockSpaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

			if (ImGui::MenuItem("Exit"))
				Moza::Application::Get().Close();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settings");
	
	//ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });

	ImGui::End();

	ImGui::End();
}

void Sandbox2D::OnEvent(Moza::Event& e)
{
	m_CameraController.OnEvent(e);
}
