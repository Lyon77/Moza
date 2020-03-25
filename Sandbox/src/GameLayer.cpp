#include "GameLayer.h"

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

	// Offset so they don't overlap
	glm::vec2 wabitoOffset =   glm::vec2(-0.12, -0.12);
	glm::vec2 natsukiOffset =  glm::vec2( 0.12, -0.12);
	glm::vec2 kenjiOffset =    glm::vec2( 0.12,  0.12);
	glm::vec2 kazutoOffset =   glm::vec2(-0.12,  0.12);

	std::vector<int> wabitoDestinations;
	wabitoDestinations.push_back(6);
	wabitoDestinations.push_back(9);
	wabitoDestinations.push_back(11);
	wabitoDestinations.push_back(12);
	wabitoDestinations.push_back(18);
	wabitoDestinations.push_back(20);
	wabitoDestinations.push_back(26);
	wabitoDestinations.push_back(28);
	wabitoDestinations.push_back(30);
	wabitoDestinations.push_back(30);
	wabitoDestinations.push_back(35);
	wabitoDestinations.push_back(41);
	wabitoDestinations.push_back(44);
	wabitoDestinations.push_back(48);
	wabitoDestinations.push_back(50);

	std::vector<int> natsukiDestinations;
	natsukiDestinations.push_back(3);
	natsukiDestinations.push_back(6);
	natsukiDestinations.push_back(10);
	natsukiDestinations.push_back(14);
	natsukiDestinations.push_back(17);
	natsukiDestinations.push_back(22);
	natsukiDestinations.push_back(27);
	natsukiDestinations.push_back(30);
	natsukiDestinations.push_back(31);
	natsukiDestinations.push_back(35);
	natsukiDestinations.push_back(38);
	natsukiDestinations.push_back(40);


	std::vector<int> kenjiDestinations;
	kenjiDestinations.push_back(1);
	kenjiDestinations.push_back(3);
	kenjiDestinations.push_back(7);
	kenjiDestinations.push_back(9);
	kenjiDestinations.push_back(14);
	kenjiDestinations.push_back(15);
	kenjiDestinations.push_back(15);
	kenjiDestinations.push_back(21);
	kenjiDestinations.push_back(26);
	kenjiDestinations.push_back(30);
	kenjiDestinations.push_back(36);
	kenjiDestinations.push_back(41);
	kenjiDestinations.push_back(46);
	kenjiDestinations.push_back(50);
	kenjiDestinations.push_back(51);


	std::vector<int> kazutoDestinations;
	kazutoDestinations.push_back(2);
	kazutoDestinations.push_back(6);
	kazutoDestinations.push_back(11);
	kazutoDestinations.push_back(12);
	kazutoDestinations.push_back(17);
	kazutoDestinations.push_back(19);
	kazutoDestinations.push_back(24);
	kazutoDestinations.push_back(29);
	kazutoDestinations.push_back(32);
	kazutoDestinations.push_back(35);
	kazutoDestinations.push_back(41);
	kazutoDestinations.push_back(43);
	kazutoDestinations.push_back(45);
	kazutoDestinations.push_back(50);
	kazutoDestinations.push_back(51);


	m_Board   = Moza::Texture2D::Create("assets/textures/characters/board.png");
	m_LoveMachine = Moza::Texture2D::Create("assets/textures/characters/LoveMachine.png");

	m_Wabito = Moza::CreateRef<Player>("assets/textures/characters/Wabito.png", glm::vec2(-3.897f, 1.691f), wabitoDestinations, wabitoOffset);
	m_Natsuki = Moza::CreateRef<Player>("assets/textures/characters/Natsuki.png", glm::vec2(-3.897f, 1.691f), natsukiDestinations, natsukiOffset);
	m_Kenji = Moza::CreateRef<Player>("assets/textures/characters/Kenji.png", glm::vec2(-3.897f, 1.691f), kenjiDestinations, kenjiOffset);
	m_Kazuto  = Moza::CreateRef<Player>("assets/textures/characters/Kazuto.png", glm::vec2(-3.897f, 1.691f), kazutoDestinations, kazutoOffset);

	std::vector<int> pointValues;
	pointValues.push_back(20);
	pointValues.push_back(5);

	m_Cards = Moza::CreateRef<CardEvent>("assets/textures/cards/", pointValues);
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
		Moza::Renderer2D::DrawQuad({  0.0f, 0.0f, 0.0f }, { 10.0f, 5.0f }, m_Board,  { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);

		if (m_Revealed)
			Moza::Renderer2D::DrawQuad({ 3.897f, -1.397f, 0.1f }, { 1.0f, 1.0f }, m_LoveMachine,  { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
		
		m_Wabito->Update(ts);
		m_Natsuki->Update(ts);
		m_Kenji->Update(ts);
		m_Kazuto->Update(ts);

		if (m_DisplayCards && m_CardIndex < 2 && m_CardIndex > -1) 
			m_Cards->Draw(m_CardIndex);


		Moza::Renderer2D::EndScene();
	}
}

void GameLayer::OnImGuiRender()
{
	MZ_PROFILE_FUNCTION();

	ImGui::Begin("Menu");

	ImGui::TextWrapped("Welcome to Love Machine. This is a Co-operative game where the 4 players try to defeat Love Machine from destroying the Internet. In order to defeat him, you must unlock his location and discover his weakness. The chance spaces will reveal more information and lead you to him. Good Luck!");

	std::string wabitoPoints = "Wabito: " + std::to_string(m_Wabito->points);
	std::string natsukiPoints = "Natsuki: " + std::to_string(m_Natsuki->points);
	std::string kenjiPoints = "Kenji: " + std::to_string(m_Kenji->points);
	std::string kazutoPoints = "Kazuto: " + std::to_string(m_Kazuto->points);
	ImGui::TextWrapped(wabitoPoints.c_str());
	ImGui::TextWrapped(natsukiPoints.c_str());
	ImGui::TextWrapped(kenjiPoints.c_str());
	ImGui::TextWrapped(kazutoPoints.c_str());

	//ImGui::SliderFloat2("Position", &m_Position.x, -5.0f, 5.0f);

	bool moving = m_Wabito->IsMoving || m_Natsuki->IsMoving || m_Kenji->IsMoving || m_Kazuto->IsMoving;

	static int roll = 0;
	if (!moving && ImGui::Button("Roll"))
	{
		std::vector<Moza::Ref<Player>> players;
		players.push_back(m_Wabito);
		players.push_back(m_Natsuki);
		players.push_back(m_Kenji);
		players.push_back(m_Kazuto);

		roll = players.at(m_Turn)->Move();
		int index = players.at(m_Turn)->locationIndex;

		if (index % 3 == 0 || index % 10 == 0 || index == 1 || index == 2) {
			m_CardIndex++;
			m_DisplayCards = true;
			players.at(m_Turn)->points += m_Cards->Value(m_CardIndex);
		}
		if (index == 40)
		{
			m_Revealed = true;
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
