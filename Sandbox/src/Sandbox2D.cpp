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
	}
}

void Sandbox2D::OnImGuiRender()
{
	MZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	


	ImGui::End();
}

void Sandbox2D::OnEvent(Moza::Event& e)
{
	m_CameraController.OnEvent(e);
}
