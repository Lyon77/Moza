#pragma once

#include "Moza/Core/Core.h"
#include "Moza/Core/Layer.h"

namespace Moza
{
	//not a regular stack because overlays can be placed in the latter half
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		//stack functions
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);


		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}
