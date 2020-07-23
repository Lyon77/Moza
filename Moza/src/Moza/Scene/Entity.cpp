#include "mzpch.h"
#include "Moza/Scene/Entity.h"

namespace Moza
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene)
	{
	}
}