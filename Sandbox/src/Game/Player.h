#pragma once

#include "Moza.h"

class Player
{
public:
	Player(const std::string& path, glm::vec2 start, std::vector<glm::vec3> destinations);
	~Player() = default;

	void Update(Moza::Timestep ts);
	int Move();

	bool IsMoving;
private:
	Moza::Ref<Moza::Texture2D> m_Player;

	int m_Position;
	glm::vec2 m_Location;
	glm::vec2 m_Destination;
	std::vector<glm::vec3> m_Locations;
};