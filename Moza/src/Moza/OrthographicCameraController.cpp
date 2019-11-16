#include "mzpch.h"
#include "OrthographicCameraController.h"

#include "Moza/Input.h"
#include "Moza/KeyCodes.h"

namespace Moza
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{

	}
	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		//move camera
		if (Input::IsKeyPressed(MZ_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(MZ_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(MZ_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(MZ_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;

		if (m_Rotation) 
		{
			//positive is clockwise rotation for the camera
			if (Moza::Input::IsKeyPressed(MZ_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			else if (Moza::Input::IsKeyPressed(MZ_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			
			m_Camera.SetRotation(m_CameraRotation);
		}
		
		m_Camera.SetPosition(m_CameraPosition);

		// Set translation speed so that the speed scales with the zoom
		m_CameraTranslationSpeed = m_ZoomLevel;
	}
	void OrthographicCameraController::OnEvent(Event & e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(MZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(MZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent & e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent & e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
}