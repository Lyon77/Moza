#include "Player.h"

Player::Player(const std::string& path, glm::vec2 start, std::vector<int> destinations, glm::vec2 offset)
	: m_Location(start + offset), m_Destination(start + offset), m_Locations(destinations), IsMoving(false), points(10), m_Offset(offset), m_Index(0), locationIndex(0)
{
	m_Player = Moza::Texture2D::Create(path);
}

void Player::Update(Moza::Timestep ts)
{
	glm::vec2 distance = m_Destination - m_Location;
	//m_Location = m_Destination;
	if (std::abs(distance.x) + std::abs(distance.y) > 0.03f) // To ensure that the piece doesn't keep encroaching the destination
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
	if (m_Index >= m_Locations.size())
		return 0;

	int out = m_Index == 0 ? m_Locations.at(m_Index) : m_Locations.at(m_Index) - m_Locations.at(m_Index - 1);

	m_Destination = m_Points[m_Locations.at(m_Index) - 1] + m_Offset;

	locationIndex = m_Locations.at(m_Index);
	m_Index++;

	return out;
}