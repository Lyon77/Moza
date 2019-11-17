#pragma once

#include "Moza/Core/Layer.h"

#include "Moza/Events/MouseEvent.h"
#include "Moza/Events/KeyEvent.h"
#include "Moza/Events/ApplicationEvent.h"

namespace Moza
{
	class MOZA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time;
	};
}