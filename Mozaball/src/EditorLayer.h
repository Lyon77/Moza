#pragma once

#include "Moza.h"
#include "Panels/SceneHierarchyPanel.h"

#include "Moza/Renderer/EditorCamera.h"

namespace Moza
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

		OrthographicCameraController m_CameraController;

		Ref<VertexArray> m_SquareVertexArray;
		Ref<Shader> m_FlatColorShader;

		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		Entity m_HoveredEntity;

		bool m_PrimaryCamera = true;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0, 0 };
		glm::vec2 m_ViewportBounds[2];

		EditorCamera m_EditorCamera;

		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_Stairs;

		// Gizmo
		int m_GizmoType = -1;

		// Texture Map
		uint32_t m_MapWidth;
		uint32_t m_MapHeight;
		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}