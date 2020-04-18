#include "ParticleLayer.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


ParticleLayer::ParticleLayer()
	: Layer("ParticleLayer"), m_CameraController(1280.0f / 720.0f, true),
	m_StartColor({ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f }), m_EndColor({ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f }), m_LifeTime(1.0f)
{
}

void ParticleLayer::OnAttach()
{
	MZ_PROFILE_FUNCTION();

	m_Particle.ColorBegin = m_StartColor;
	m_Particle.ColorEnd = m_EndColor;
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = m_LifeTime;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void ParticleLayer::OnDetach()
{
	MZ_PROFILE_FUNCTION();

}

void ParticleLayer::OnUpdate(Moza::Timestep ts)
{
	MZ_PROFILE_FUNCTION();

	// update Camera
	{
		MZ_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// update Render
	Moza::Renderer2D::ResetStats();

	{
		MZ_PROFILE_SCOPE("RendererPrep");
		Moza::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Moza::RendererCommand::Clear();
	}

	{ 
		MZ_PROFILE_SCOPE("RendererDraw");

		Moza::Renderer2D::BeginScene(m_CameraController.GetCamera());

		if (Moza::Input::IsMouseButtonPressed(MZ_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Moza::Input::GetMousePosition();
			auto width = Moza::Application::Get().GetWindow().GetWidth();
			auto height = Moza::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();

			m_Particle.Position = { x + pos.x, y + pos.y };
			m_Particle.ColorBegin = m_StartColor;
			m_Particle.ColorEnd = m_EndColor;
			m_Particle.LifeTime = m_LifeTime;

			for (int i = 0; i < 5; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender2D(m_CameraController.GetCamera());

		Moza::Renderer2D::EndScene();
	}
}

void ParticleLayer::OnImGuiRender()
{
	MZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	ImGui::ColorEdit3("Start Color", glm::value_ptr(m_StartColor));
	ImGui::ColorEdit3("End Color", glm::value_ptr(m_EndColor));
	ImGui::DragFloat("Life Time", &m_LifeTime, 0.01f,  0.1f, 5.0f);

	//Stats
	Moza::Renderer2D::Statistics stats = Moza::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}

void ParticleLayer::OnEvent(Moza::Event& e)
{
	m_CameraController.OnEvent(e);
}
