#pragma once

#include "Moza/Core/Core.h"
#include "Moza/Core/Timestep.h"
#include "Moza/Renderer/OrthographicCamera.h"
#include "Moza/Renderer/Shader.h"

#include <glm/glm.hpp>
#include <vector>

namespace Moza
{
	struct ParticleProps
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float LifeTime = 1.0f;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem(uint32_t maxParticles = 10000);

		void OnUpdate(Timestep ts);
		void OnRender2D(OrthographicCamera& camera);

		void Emit(const ParticleProps& particleProps);
	private:
		struct Particle
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};
		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = 999;

		uint32_t m_QuadVA = 0;
		Scope<Shader> m_ParticleShader;
		int m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;
	};
}