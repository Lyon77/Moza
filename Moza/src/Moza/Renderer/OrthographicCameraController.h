#pragma once

#include "Moza/Renderer/OrthographicCamera.h"
#include "Moza/Core/Timestep.h"

#include "Moza/Events/ApplicationEvent.h"
#include "Moza/Events/MouseEvent.h"

namespace Moza
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		//float GetZoomLevel() { return m_ZoomLevel; }
		//void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 3.0f, m_CameraRotationSpeed = 60.0f;
	};
}