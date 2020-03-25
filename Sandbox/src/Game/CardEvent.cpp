#include "CardEvent.h"

CardEvent::CardEvent(const std::string& path, std::vector<int> points)
{
	for (int i = 0; i < points.size(); i++) {
		m_Cards.push_back(Moza::Texture2D::Create(path + "card" + std::to_string(i) + ".png"));
		m_Points.push_back(points.at(i));
	}
}

void CardEvent::Draw(unsigned int index)
{
	Moza::Renderer2D::DrawScreenQuad({ 0.0f, 0.0f}, { 4.0f, 3.0f }, m_Cards.at(index), { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
}

int CardEvent::Value(unsigned int index)
{
	if (index >= m_Points.size())
		return 0;

	return m_Points.at(index);
}
