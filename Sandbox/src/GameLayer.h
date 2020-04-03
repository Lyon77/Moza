#pragma once

#include "Moza.h"

#include "Game/Player.h"
#include "Game/CardEvent.h"

class GameLayer : public Moza::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Moza::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Moza::Event& e) override;

	bool OnKeyReleased(Moza::KeyReleasedEvent & e);

private:
	// TEMPORARY
	Moza::Ref<Moza::VertexArray> m_SquareVertexArray;
	Moza::Ref<Moza::Shader> m_FlatColorShader;

	Moza::OrthographicCameraController m_CameraController;

	Moza::Ref<Moza::Texture2D> m_Board;
	Moza::Ref<Moza::Texture2D> m_LoveMachine;
	Moza::Ref<Moza::Texture2D> m_Win;

	Moza::Ref<Player> m_Wabito;
	Moza::Ref<Player> m_Natsuki;
	Moza::Ref<Player> m_Kenji;
	Moza::Ref<Player> m_Kazuto;
	

	Moza::Ref<CardEvent> m_Cards;

	unsigned int m_Turn;
	bool m_Revealed = false;
	bool m_Won = false;

	bool m_DisplayCards;
	int m_CardIndex;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	glm::vec2 m_Position = glm::vec2(0.0f, 0.0f);
};
