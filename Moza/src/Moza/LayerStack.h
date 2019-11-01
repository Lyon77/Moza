#pragma once

#include "Moza/Core.h"
#include "Layer.h"

namespace Moza
{
	//not a regular stack because overlays can be placed in the latter half
	class MOZA_API LayerStack
	{
	public:
		LayerStack();
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
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}