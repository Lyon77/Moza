#include "Player.h"

Player::Player(const std::string& path, glm::vec2 start, std::vector<glm::vec3> destinations)
	: m_Location(start), m_Destination(start), m_Locations(destinations), m_Position(0), IsMoving(false)
{
	m_Player = Moza::Texture2D::Create(path);
}

void Player::Update(Moza::Timestep ts)
{
	glm::vec2 distance = m_Destination - m_Location;
	
	if (std::abs(distance.x) + std::abs(distance.y) > 0.05f) // To ensure that the piece doesn't keep encroaching the destination
	{
		glm::vec2 displacement = distance * glm::vec2(ts * 2.0f);
		m_Location += displacement;
		IsMoving = true;
	}
	else
	{
		m_Location = m_Destination;
		IsMoving = false;
	}

	Moza::Renderer2D::DrawQuad({ m_Location.x, m_Location.y, 0.1f }, { 0.25f, 0.25f }, m_Player, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
}

int Player::Move()
{
	m_Destination = glm::vec2(m_Locations[m_Position].x, m_Locations[m_Position].y);
	int out = (int)m_Locations[m_Position].z;

	m_Position += m_Locations.size() > ((int)m_Position + 1) ? 1 : 0;

	return out;
}