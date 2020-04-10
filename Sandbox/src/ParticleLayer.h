#pragma once

#include "Moza.h"

class ParticleLayer : public Moza::Layer
{
public:
	ParticleLayer();
	virtual ~ParticleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Moza::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Moza::Event& e) override;

private:
	Moza::OrthographicCameraController m_CameraController;

	Moza::ParticleProps m_Particle;
	Moza::ParticleSystem m_ParticleSystem;
};