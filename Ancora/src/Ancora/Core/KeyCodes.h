#pragma once

namespace Ancora
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space               = 32,
		Apostrophe          = 39, /* ' */
		Comma               = 44, /* , */
		Minus               = 45, /* - */
		Period              = 46, /* . */
		Slash               = 47, /* / */

		D0                  = 48, /* 0 */
		D1                  = 49, /* 1 */
		D2                  = 50, /* 2 */
		D3                  = 51, /* 3 */
		D4                  = 52, /* 4 */
		D5                  = 53, /* 5 */
		D6                  = 54, /* 6 */
		D7                  = 55, /* 7 */
		D8                  = 56, /* 8 */
		D9                  = 57, /* 9 */

		Semicolon           = 59, /* ; */
		Equal               = 61, /* = */

		A                   = 65,
		B                   = 66,
		C                   = 67,
		D                   = 68,
		E                   = 69,
		F                   = 70,
		G                   = 71,
		H                   = 72,
		I                   = 73,
		J                   = 74,
		K                   = 75,
		L                   = 76,
		M                   = 77,
		N                   = 78,
		O                   = 79,
		P                   = 80,
		Q                   = 81,
		R                   = 82,
		S                   = 83,
		T                   = 84,
		U                   = 85,
		V                   = 86,
		W                   = 87,
		X                   = 88,
		Y                   = 89,
		Z                   = 90,

		LeftBracket         = 91,  /* [ */
		Backslash           = 92,  /* \ */
		RightBracket        = 93,  /* ] */
		GraveAccent         = 96,  /* ` */

		World1              = 161, /* non-US #1 */
		World2              = 162, /* non-US #2 */

		/* Function keys */
		Escape              = 256,
		Enter               = 257,
		Tab                 = 258,
		Backspace           = 259,
		Insert              = 260,
		Delete              = 261,
		Right               = 262,
		Left                = 263,
		Down                = 264,
		Up                  = 265,
		PageUp              = 266,
		PageDown            = 267,
		Home                = 268,
		End                 = 269,
		CapsLock            = 280,
		ScrollLock          = 281,
		NumLock             = 282,
		PrintScreen         = 283,
		Pause               = 284,
		F1                  = 290,
		F2                  = 291,
		F3                  = 292,
		F4                  = 293,
		F5                  = 294,
		F6                  = 295,
		F7                  = 296,
		F8                  = 297,
		F9                  = 298,
		F10                 = 299,
		F11                 = 300,
		F12                 = 301,
		F13                 = 302,
		F14                 = 303,
		F15                 = 304,
		F16                 = 305,
		F17                 = 306,
		F18                 = 307,
		F19                 = 308,
		F20                 = 309,
		F21                 = 310,
		F22                 = 311,
		F23                 = 312,
		F24                 = 313,
		F25                 = 314,

		/* Keypad */
		KP0                 = 320,
		KP1                 = 321,
		KP2                 = 322,
		KP3                 = 323,
		KP4                 = 324,
		KP5                 = 325,
		KP6                 = 326,
		KP7                 = 327,
		KP8                 = 328,
		KP9                 = 329,
		KPDecimal           = 330,
		KPDivide            = 331,
		KPMultiply          = 332,
		KPSubtract          = 333,
		KPAdd               = 334,
		KPEnter             = 335,
		KPEqual             = 336,

		LeftShift           = 340,
		LeftControl         = 341,
		LeftAlt             = 342,
		LeftSuper           = 343,
		RightShift          = 344,
		RightControl        = 345,
		RightAlt            = 346,
		RightSuper          = 347,
		Menu                = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define AE_KEY_SPACE           ::Ancora::Key::Space
#define AE_KEY_APOSTROPHE      ::Ancora::Key::Apostrophe    /* ' */
#define AE_KEY_COMMA           ::Ancora::Key::Comma         /* , */
#define AE_KEY_MINUS           ::Ancora::Key::Minus         /* - */
#define AE_KEY_PERIOD          ::Ancora::Key::Period        /* . */
#define AE_KEY_SLASH           ::Ancora::Key::Slash         /* / */
#define AE_KEY_0               ::Ancora::Key::D0
#define AE_KEY_1               ::Ancora::Key::D1
#define AE_KEY_2               ::Ancora::Key::D2
#define AE_KEY_3               ::Ancora::Key::D3
#define AE_KEY_4               ::Ancora::Key::D4
#define AE_KEY_5               ::Ancora::Key::D5
#define AE_KEY_6               ::Ancora::Key::D6
#define AE_KEY_7               ::Ancora::Key::D7
#define AE_KEY_8               ::Ancora::Key::D8
#define AE_KEY_9               ::Ancora::Key::D9
#define AE_KEY_SEMICOLON       ::Ancora::Key::Semicolon     /* ; */
#define AE_KEY_EQUAL           ::Ancora::Key::Equal         /* = */
#define AE_KEY_A               ::Ancora::Key::A
#define AE_KEY_B               ::Ancora::Key::B
#define AE_KEY_C               ::Ancora::Key::C
#define AE_KEY_D               ::Ancora::Key::D
#define AE_KEY_E               ::Ancora::Key::E
#define AE_KEY_F               ::Ancora::Key::F
#define AE_KEY_G               ::Ancora::Key::G
#define AE_KEY_H               ::Ancora::Key::H
#define AE_KEY_I               ::Ancora::Key::I
#define AE_KEY_J               ::Ancora::Key::J
#define AE_KEY_K               ::Ancora::Key::K
#define AE_KEY_L               ::Ancora::Key::L
#define AE_KEY_M               ::Ancora::Key::M
#define AE_KEY_N               ::Ancora::Key::N
#define AE_KEY_O               ::Ancora::Key::O
#define AE_KEY_P               ::Ancora::Key::P
#define AE_KEY_Q               ::Ancora::Key::Q
#define AE_KEY_R               ::Ancora::Key::R
#define AE_KEY_S               ::Ancora::Key::S
#define AE_KEY_T               ::Ancora::Key::T
#define AE_KEY_U               ::Ancora::Key::U
#define AE_KEY_V               ::Ancora::Key::V
#define AE_KEY_W               ::Ancora::Key::W
#define AE_KEY_X               ::Ancora::Key::X
#define AE_KEY_Y               ::Ancora::Key::Y
#define AE_KEY_Z               ::Ancora::Key::Z
#define AE_KEY_LEFT_BRACKET    ::Ancora::Key::LeftBracket   /* [ */
#define AE_KEY_BACKSLASH       ::Ancora::Key::Backslash     /* \ */
#define AE_KEY_RIGHT_BRACKET   ::Ancora::Key::RightBracket  /* ] */
#define AE_KEY_GRAVE_ACCENT    ::Ancora::Key::GraveAccent   /* ` */
#define AE_KEY_WORLD_1         ::Ancora::Key::World1        /* non-US #1 */
#define AE_KEY_WORLD_2         ::Ancora::Key::World2        /* non-US #2 */

/* Function keys */
#define AE_KEY_ESCAPE          ::Ancora::Key::Escape
#define AE_KEY_ENTER           ::Ancora::Key::Enter
#define AE_KEY_TAB             ::Ancora::Key::Tab
#define AE_KEY_BACKSPACE       ::Ancora::Key::Backspace
#define AE_KEY_INSERT          ::Ancora::Key::Insert
#define AE_KEY_DELETE          ::Ancora::Key::Delete
#define AE_KEY_RIGHT           ::Ancora::Key::Right
#define AE_KEY_LEFT            ::Ancora::Key::Left
#define AE_KEY_DOWN            ::Ancora::Key::Down
#define AE_KEY_UP              ::Ancora::Key::Up
#define AE_KEY_PAGE_UP         ::Ancora::Key::PageUp
#define AE_KEY_PAGE_DOWN       ::Ancora::Key::PageDown
#define AE_KEY_HOME            ::Ancora::Key::Home
#define AE_KEY_END             ::Ancora::Key::End
#define AE_KEY_CAPS_LOCK       ::Ancora::Key::CapsLock
#define AE_KEY_SCROLL_LOCK     ::Ancora::Key::ScrollLock
#define AE_KEY_NUM_LOCK        ::Ancora::Key::NumLock
#define AE_KEY_PRINT_SCREEN    ::Ancora::Key::PrintScreen
#define AE_KEY_PAUSE           ::Ancora::Key::Pause
#define AE_KEY_F1              ::Ancora::Key::F1
#define AE_KEY_F2              ::Ancora::Key::F2
#define AE_KEY_F3              ::Ancora::Key::F3
#define AE_KEY_F4              ::Ancora::Key::F4
#define AE_KEY_F5              ::Ancora::Key::F5
#define AE_KEY_F6              ::Ancora::Key::F6
#define AE_KEY_F7              ::Ancora::Key::F7
#define AE_KEY_F8              ::Ancora::Key::F8
#define AE_KEY_F9              ::Ancora::Key::F9
#define AE_KEY_F10             ::Ancora::Key::F10
#define AE_KEY_F11             ::Ancora::Key::F11
#define AE_KEY_F12             ::Ancora::Key::F12
#define AE_KEY_F13             ::Ancora::Key::F13
#define AE_KEY_F14             ::Ancora::Key::F14
#define AE_KEY_F15             ::Ancora::Key::F15
#define AE_KEY_F16             ::Ancora::Key::F16
#define AE_KEY_F17             ::Ancora::Key::F17
#define AE_KEY_F18             ::Ancora::Key::F18
#define AE_KEY_F19             ::Ancora::Key::F19
#define AE_KEY_F20             ::Ancora::Key::F20
#define AE_KEY_F21             ::Ancora::Key::F21
#define AE_KEY_F22             ::Ancora::Key::F22
#define AE_KEY_F23             ::Ancora::Key::F23
#define AE_KEY_F24             ::Ancora::Key::F24
#define AE_KEY_F25             ::Ancora::Key::F25

/* Keypad */
#define AE_KEY_KP_0            ::Ancora::Key::KP0
#define AE_KEY_KP_1            ::Ancora::Key::KP1
#define AE_KEY_KP_2            ::Ancora::Key::KP2
#define AE_KEY_KP_3            ::Ancora::Key::KP3
#define AE_KEY_KP_4            ::Ancora::Key::KP4
#define AE_KEY_KP_5            ::Ancora::Key::KP5
#define AE_KEY_KP_6            ::Ancora::Key::KP6
#define AE_KEY_KP_7            ::Ancora::Key::KP7
#define AE_KEY_KP_8            ::Ancora::Key::KP8
#define AE_KEY_KP_9            ::Ancora::Key::KP9
#define AE_KEY_KP_DECIMAL      ::Ancora::Key::KPDecimal
#define AE_KEY_KP_DIVIDE       ::Ancora::Key::KPDivide
#define AE_KEY_KP_MULTIPLY     ::Ancora::Key::KPMultiply
#define AE_KEY_KP_SUBTRACT     ::Ancora::Key::KPSubtract
#define AE_KEY_KP_ADD          ::Ancora::Key::KPAdd
#define AE_KEY_KP_ENTER        ::Ancora::Key::KPEnter
#define AE_KEY_KP_EQUAL        ::Ancora::Key::KPEqual

#define AE_KEY_LEFT_SHIFT      ::Ancora::Key::LeftShift
#define AE_KEY_LEFT_CONTROL    ::Ancora::Key::LeftControl
#define AE_KEY_LEFT_ALT        ::Ancora::Key::LeftAlt
#define AE_KEY_LEFT_SUPER      ::Ancora::Key::LeftSuper
#define AE_KEY_RIGHT_SHIFT     ::Ancora::Key::RightShift
#define AE_KEY_RIGHT_CONTROL   ::Ancora::Key::RightControl
#define AE_KEY_RIGHT_ALT       ::Ancora::Key::RightAlt
#define AE_KEY_RIGHT_SUPER     ::Ancora::Key::RightSuper
#define AE_KEY_MENU            ::Ancora::Key::Menu
