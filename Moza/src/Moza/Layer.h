#pragma once

#include "Moza/Core.h"
#include "Moza/Events/Event.h"

namespace Moza 
{
	class MOZA_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
		
		// for debug purposes because it's better practice to only use for debug
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}
