#include "GameLayer.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GameLayer::GameLayer()
	: Layer("GameLayer"), m_CameraController(1280.0f / 720.0f, true), m_Turn(0), m_DisplayCards(false), m_CardIndex(-1)
{
}

void GameLayer::OnAttach()
{
	MZ_PROFILE_FUNCTION();

	std::vector<glm::vec3> kazutoDestinations;
	kazutoDestinations.push_back(glm::vec3(0.0f));
	kazutoDestinations.push_back(glm::vec3(2.0f));

	m_Board   = Moza::Texture2D::Create("assets/textures/Checkerboard.png");

	m_Wabito = Moza::CreateRef<Player>("assets/textures/characters/Wabito.png", glm::vec2(-0.5f, -1.0f), kazutoDestinations);
	m_Natsuki = Moza::CreateRef<Player>("assets/textures/characters/Natsuki.png", glm::vec2(-0.5f, -0.5f), kazutoDestinations);
	m_Kenji = Moza::CreateRef<Player>("assets/textures/characters/Kenji.png", glm::vec2(-0.5f, 0.0f), kazutoDestinations);
	m_Kazuto  = Moza::CreateRef<Player>("assets/textures/characters/Kazuto.png", glm::vec2(-0.5f, 0.5f), kazutoDestinations);

	m_Cards = Moza::CreateRef<CardEvent>("assets/textures/ChernoLogo.png");
}

void GameLayer::OnDetach()
{
	MZ_PROFILE_FUNCTION();

}

void GameLayer::OnUpdate(Moza::Timestep ts)
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
		Moza::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Moza::Renderer2D::DrawQuad({  0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, m_Board,  { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
		
		m_Wabito->Update(ts);
		m_Natsuki->Update(ts);
		m_Kenji->Update(ts);
		m_Kazuto->Update(ts);

		if (m_DisplayCards)
			m_Cards->Draw(m_CardIndex);

		Moza::Renderer2D::EndScene();
	}
}

void GameLayer::OnImGuiRender()
{
	MZ_PROFILE_FUNCTION();

	ImGui::Begin("Menu");

	ImGui::TextWrapped("Welcome to Love Machine. This is a Co-operative game where the 4 players try to defeat Love Machine from destroying the Internet. In order to defeat him, you must unlock his location and discover his weakness. The chance spaces will reveal more information and lead you to him. Good Luck!");

	bool moving = m_Wabito->IsMoving || m_Natsuki->IsMoving || m_Kenji->IsMoving || m_Kazuto->IsMoving;

	static int roll = 0;
	if (!moving && ImGui::Button("Roll"))
	{
		switch (m_Turn)
		{
		case 0:
			roll = m_Wabito->Move();
			break;
		case 1:
			roll = m_Natsuki->Move();
			break;
		case 2:
			roll = m_Kenji->Move();
			break;
		case 3:
			roll = m_Kazuto->Move();
			break;
		default:
			MZ_ERROR("No Player Selected!");
		}

		m_Turn = (m_Turn + 1) % 4;
	}

	if (moving) 
	{
		std::string output = "You rolled a " + std::to_string(roll);
		ImGui::TextWrapped(output.c_str());
	}

	ImGui::End();
}

void GameLayer::OnEvent(Moza::Event& e)
{
	m_CameraController.OnEvent(e);

	Moza::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Moza::KeyReleasedEvent>(MZ_BIND_EVENT_FN(GameLayer::OnKeyReleased));
}

bool GameLayer::OnKeyReleased(Moza::KeyReleasedEvent& e)
{
	if (e.GetKeyCode() == MZ_KEY_C)
		m_DisplayCards = !m_DisplayCards;

	if (m_DisplayCards && (m_CardIndex - 1 > -1) && e.GetKeyCode() == MZ_KEY_LEFT)
		m_CardIndex--;

	if (m_DisplayCards && e.GetKeyCode() == MZ_KEY_RIGHT)
		m_CardIndex++;

	return false;
}
