#pragma once

#include "Moza.h"

class CardEvent
{
public:
	CardEvent(const std::string& path, std::vector<int> points);
	~CardEvent() = default;

	void Draw(unsigned int index);
	int Value(unsigned int index);
private:
	Moza::Ref<Moza::Texture2D> m_Cards[10];
	Moza::Ref<Moza::Shader> m_Shader;

	std::vector<int> m_Points;
};
