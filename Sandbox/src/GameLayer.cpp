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

	glm::vec3 destinations[50] = {
		//Row 1
		glm::vec3(-2.712, 1.691, 1.0),
		glm::vec3(-2.059, 1.691, 1.0),
		glm::vec3(-1.471, 1.691, 1.0),
		glm::vec3(-0.882, 1.691, 1.0),
		glm::vec3(-0.294, 1.691, 1.0),
		glm::vec3( 0.294, 1.691, 1.0),
		glm::vec3( 0.882, 1.691, 1.0),
		glm::vec3( 1.471, 1.691, 1.0),
		glm::vec3( 2.059, 1.691, 1.0),
		glm::vec3( 2.712, 1.691, 1.0),

		//Row 2
		glm::vec3( 2.712, 0.882, 1.0),
		glm::vec3( 2.059, 0.882, 1.0),
		glm::vec3( 1.471, 0.882, 1.0),
		glm::vec3( 0.882, 0.882, 1.0),
		glm::vec3( 0.294, 0.882, 1.0),
		glm::vec3(-0.294, 0.882, 1.0),
		glm::vec3(-0.882, 0.882, 1.0),
		glm::vec3(-1.471, 0.882, 1.0),
		glm::vec3(-2.059, 0.882, 1.0),
		glm::vec3(-2.712, 0.882, 1.0),

		// Row 3
		glm::vec3(-2.712, 0.100, 1.0),
		glm::vec3(-2.059, 0.100, 1.0),
		glm::vec3(-1.471, 0.100, 1.0),
		glm::vec3(-0.882, 0.100, 1.0),
		glm::vec3(-0.294, 0.100, 1.0),
		glm::vec3( 0.294, 0.100, 1.0),
		glm::vec3( 0.882, 0.100, 1.0),
		glm::vec3( 1.471, 0.100, 1.0),
		glm::vec3( 2.059, 0.100, 1.0),
		glm::vec3( 2.712, 0.100, 1.0),


		// Row 4
		glm::vec3( 2.712, -0.662, 1.0),
		glm::vec3( 2.059, -0.662, 1.0),
		glm::vec3( 1.471, -0.662, 1.0),
		glm::vec3( 0.882, -0.662, 1.0),
		glm::vec3( 0.294, -0.662, 1.0),
		glm::vec3(-0.294, -0.662, 1.0),
		glm::vec3(-0.882, -0.662, 1.0),
		glm::vec3(-1.471, -0.662, 1.0),
		glm::vec3(-2.059, -0.662, 1.0),
		glm::vec3(-2.712, -0.662, 1.0),

		// Row 5
		glm::vec3(-2.712, -1.472, 1.0),
		glm::vec3(-2.059, -1.472, 1.0),
		glm::vec3(-1.471, -1.472, 1.0),
		glm::vec3(-0.882, -1.472, 1.0),
		glm::vec3(-0.294, -1.472, 1.0),
		glm::vec3( 0.294, -1.472, 1.0),
		glm::vec3( 0.882, -1.472, 1.0),
		glm::vec3( 1.471, -1.472, 1.0),
		glm::vec3( 2.059, -1.472, 1.0),
		glm::vec3( 2.712, -1.472, 1.0)
	};

	// Offset so they don't overlap
	glm::vec3 wabitoOffset =   glm::vec3(-0.12, -0.12, 0.0f);
	glm::vec3 natsukiOffset =  glm::vec3( 0.12, -0.12, 0.0f);
	glm::vec3 kenjiOffset =    glm::vec3( 0.12,  0.12, 0.0f);
	glm::vec3 kazutoOffset =   glm::vec3(-0.12,  0.12, 0.0f);

	std::vector<glm::vec3> wabitoDestinations;
	wabitoDestinations.push_back(destinations[0] + wabitoOffset);
	wabitoDestinations.push_back(destinations[1] + wabitoOffset);

	std::vector<glm::vec3> natsukiDestinations;
	natsukiDestinations.push_back(destinations[0] + natsukiOffset);
	natsukiDestinations.push_back(destinations[1] + natsukiOffset);

	std::vector<glm::vec3> kenjiDestinations;
	kenjiDestinations.push_back(destinations[0] + kenjiOffset);
	kenjiDestinations.push_back(destinations[1] + kenjiOffset);

	std::vector<glm::vec3> kazutoDestinations;
	kazutoDestinations.push_back(destinations[0] + kazutoOffset);
	kazutoDestinations.push_back(destinations[1] + kazutoOffset);

	m_Board   = Moza::Texture2D::Create("assets/textures/characters/board.png");

	m_Wabito = Moza::CreateRef<Player>("assets/textures/characters/Wabito.png", glm::vec2(-3.897f, 1.691f) + glm::vec2(wabitoOffset.x, wabitoOffset.y), wabitoDestinations);
	m_Natsuki = Moza::CreateRef<Player>("assets/textures/characters/Natsuki.png", glm::vec2(-3.897f, 1.691f) + glm::vec2(natsukiOffset.x, natsukiOffset.y), natsukiDestinations);
	m_Kenji = Moza::CreateRef<Player>("assets/textures/characters/Kenji.png", glm::vec2(-3.897f, 1.691f) + glm::vec2(kenjiOffset.x, kenjiOffset.y), kenjiDestinations);
	m_Kazuto  = Moza::CreateRef<Player>("assets/textures/characters/Kazuto.png", glm::vec2(-3.897f, 1.691f) + glm::vec2(kazutoOffset.x, kazutoOffset.y), kazutoDestinations);
	m_LoveMachine  = Moza::CreateRef<Player>("assets/textures/characters/LoveMachine.png", glm::vec2(3.897f, -1.397f), kazutoDestinations);

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
		Moza::Renderer2D::DrawQuad({  0.0f, 0.0f, 0.0f }, { 10.0f, 5.0f }, m_Board,  { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
		
		//m_Wabito->m_Location = m_Position;
		//m_Wabito->m_Destination = m_Position;
		m_Wabito->Update(ts);
		m_Natsuki->Update(ts);
		m_Kenji->Update(ts);
		m_Kazuto->Update(ts);
		m_LoveMachine->Update(ts);

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

	ImGui::SliderFloat2("Position", &m_Position.x, -5.0f, 5.0f);

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
