#include "CardEvent.h"

CardEvent::CardEvent(const std::string& path)
{
	m_Cards = Moza::Texture2D::Create(path);
}

void CardEvent::Draw(unsigned int index)
{
	Moza::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.2f }, { 6.0f, 4.0f }, m_Cards, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
}
