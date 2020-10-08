#pragma once

#include "Moza/Core/Core.h"
#include "Moza/Core/Log.h"
#include "Moza/Scene/Scene.h"
#include "Moza/Scene/Entity.h"

namespace Moza
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}