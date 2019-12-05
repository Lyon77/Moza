#pragma once

#include "Moza.h"

class ExampleLayer : public Moza::Layer 
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach();
	virtual void OnDetach();

	void OnUpdate(Moza::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Moza::Event& e) override;
private:
	// Will be in Renderer in the future
	Moza::ShaderLibrary m_ShaderLibrary;

	Moza::Ref<Moza::VertexArray> m_VertexArray;
	Moza::Ref<Moza::VertexArray> m_SquareVertexArray;

	Moza::Ref<Moza::Texture2D> m_Texture, m_ChernoLogoTexture;

	Moza::OrthographicCameraController m_CameraController;

	glm::vec3 m_RenderPosition;
	float m_RenderMoveSpeed = 3.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};