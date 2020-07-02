#pragma once

#include "Moza/Core/Core.h"
#include "Moza/Core/KeyCodes.h"
#include "Moza/Core/MouseButtonCodes.h"

namespace Moza
{
	class Input
	{
	public:
		inline static bool IsKeyPressed(int keycode);

		inline static bool IsMouseButtonPressed(int button);
		inline static std::pair<float, float> GetMousePosition();
		inline static float GetMouseX();
		inline static float GetMouseY();
	};
}