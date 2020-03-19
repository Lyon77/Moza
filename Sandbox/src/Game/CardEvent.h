#pragma once

#include "Moza.h"

class CardEvent
{
public:
	CardEvent(const std::string& path);
	~CardEvent() = default;

	void Draw(unsigned int index);
private:
	Moza::Ref<Moza::Texture2D> m_Cards;
	Moza::Ref<Moza::Shader> m_Shader;
};
