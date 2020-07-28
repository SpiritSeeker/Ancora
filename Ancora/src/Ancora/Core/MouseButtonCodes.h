#pragma once

namespace Ancora
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0                = 0,
		Button1                = 1,
		Button2                = 2,
		Button3                = 3,
		Button4                = 4,
		Button5                = 5,
		Button6                = 6,
		Button7                = 7,

		ButtonLast             = Button7,
		ButtonLeft             = Button0,
		ButtonRight            = Button1,
		ButtonMiddle           = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define AE_MOUSE_BUTTON_0      ::Ancora::Mouse::Button0
#define AE_MOUSE_BUTTON_1      ::Ancora::Mouse::Button1
#define AE_MOUSE_BUTTON_2      ::Ancora::Mouse::Button2
#define AE_MOUSE_BUTTON_3      ::Ancora::Mouse::Button3
#define AE_MOUSE_BUTTON_4      ::Ancora::Mouse::Button4
#define AE_MOUSE_BUTTON_5      ::Ancora::Mouse::Button5
#define AE_MOUSE_BUTTON_6      ::Ancora::Mouse::Button6
#define AE_MOUSE_BUTTON_7      ::Ancora::Mouse::Button7
#define AE_MOUSE_BUTTON_LAST   ::Ancora::Mouse::ButtonLast
#define AE_MOUSE_BUTTON_LEFT   ::Ancora::Mouse::ButtonLeft
#define AE_MOUSE_BUTTON_RIGHT  ::Ancora::Mouse::ButtonRight
#define AE_MOUSE_BUTTON_MIDDLE ::Ancora::Mouse::ButtonMiddle
