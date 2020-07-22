#include "mzpch.h"
#include "Moza/Scene/Scene.h"

#include "Moza/Renderer/Renderer2D.h"
#include "Moza/Scene/Components.h"

namespace Moza
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}