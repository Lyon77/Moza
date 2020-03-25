#pragma once

#include "Moza.h"

class Player
{
public:
	Player(const std::string& path, glm::vec2 start, std::vector<int> destinations, glm::vec2 offset);
	~Player() = default;

	void Update(Moza::Timestep ts);
	int Move();

	bool IsMoving;
	
	int points;
	int locationIndex;
private:
	Moza::Ref<Moza::Texture2D> m_Player;

	glm::vec2 m_Offset;
	glm::vec2 m_Location;
	glm::vec2 m_Destination;
	std::vector<int> m_Locations;

	int m_Index;

	glm::vec2 m_Points[51] = {
		//Row 1
		glm::vec2(-2.712, 1.691),
		glm::vec2(-2.059, 1.691),
		glm::vec2(-1.471, 1.691),
		glm::vec2(-0.882, 1.691),
		glm::vec2(-0.294, 1.691),
		glm::vec2(0.294, 1.691),
		glm::vec2(0.882, 1.691),
		glm::vec2(1.471, 1.691),
		glm::vec2(2.059, 1.691),
		glm::vec2(2.712, 1.691),

		//Row 2
		glm::vec2(2.712, 0.882),
		glm::vec2(2.059, 0.882),
		glm::vec2(1.471, 0.882),
		glm::vec2(0.882, 0.882),
		glm::vec2(0.294, 0.882),
		glm::vec2(-0.294, 0.882),
		glm::vec2(-0.882, 0.882),
		glm::vec2(-1.471, 0.882),
		glm::vec2(-2.059, 0.882),
		glm::vec2(-2.712, 0.882),

		// Row 3
		glm::vec2(-2.712, 0.100),
		glm::vec2(-2.059, 0.100),
		glm::vec2(-1.471, 0.100),
		glm::vec2(-0.882, 0.100),
		glm::vec2(-0.294, 0.100),
		glm::vec2(0.294, 0.100),
		glm::vec2(0.882, 0.100),
		glm::vec2(1.471, 0.100),
		glm::vec2(2.059, 0.100),
		glm::vec2(2.712, 0.100),


		// Row 4
		glm::vec2(2.712, -0.662),
		glm::vec2(2.059, -0.662),
		glm::vec2(1.471, -0.662),
		glm::vec2(0.882, -0.662),
		glm::vec2(0.294, -0.662),
		glm::vec2(-0.294, -0.662),
		glm::vec2(-0.882, -0.662),
		glm::vec2(-1.471, -0.662),
		glm::vec2(-2.059, -0.662),
		glm::vec2(-2.712, -0.662),

		// Row 5
		glm::vec2(-2.712, -1.472),
		glm::vec2(-2.059, -1.472),
		glm::vec2(-1.471, -1.472),
		glm::vec2(-0.882, -1.472),
		glm::vec2(-0.294, -1.472),
		glm::vec2(0.294, -1.472),
		glm::vec2(0.882, -1.472),
		glm::vec2(1.471, -1.472),
		glm::vec2(2.059, -1.472),
		glm::vec2(2.712, -1.472),

		//End
		glm::vec2(3.897f, -1.397f)
	};
};