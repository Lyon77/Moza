#pragma once

#include "Moza.h"

class Sandbox2D : public Moza::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Moza::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Moza::Event& e) override;

private:
	// TEMPORARY
	Moza::Ref<Moza::VertexArray> m_SquareVertexArray;
	Moza::Ref<Moza::Shader> m_FlatColorShader;

	Moza::OrthographicCameraController m_CameraController;

	Moza::Ref<Moza::Texture2D> m_CheckerboardTexture;
	Moza::Ref<Moza::Texture2D> m_SpriteSheet;
	Moza::Ref<Moza::SubTexture2D> m_Stairs;
	Moza::Ref<Moza::SubTexture2D> m_Tree;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};