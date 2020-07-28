#pragma once

#include "Ancora/Core/Core.h"
#include "Ancora/Core/KeyCodes.h"
#include "Ancora/Core/MouseButtonCodes.h"

namespace Ancora {

	class ANCORA_API Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
