#include "Platform/Windows/WndWindow.h"
#include "Platform/Windows/SDL_keycode.h"

#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/Window.h"

#include "Core/ModuleManager.h"

#include "Input/Input.h"

void Input::SetMousePosition(int x, int y)
{
	//auto window = ModuleManager::Get<WndWindow>();
	//SDL_WarpMouseInWindow(window->m_SDLWindow, x, y);
}

std::pair<int, int> Input::GetMousePosition()
{
	int x, y;
	//SDL_GetMouseState(&x, &y);
	return { x, y };
}

std::pair<int, int> Input::GetMouseDelta()
{
	return { WndWindow::s_MouseDeltaX, WndWindow::s_MouseDeltaY };
}

void Input::ShowCursor(bool show)
{
	::ShowCursor(show);
}

bool Input::IsCursorVisible()
{
	CURSORINFO ci;
	ci.cbSize = sizeof(ci);
	GetCursorInfo(&ci);
	
	return ci.flags != 0;
}

void Input::Update()
{
}

int MouseCode::MC_BUTTON_1				= 1;
int MouseCode::MC_BUTTON_2				= 2;
int MouseCode::MC_BUTTON_3				= 3;
int MouseCode::MC_WHEEL_UP				= 30;
int MouseCode::MC_WHEEL_DOWN			= 31;
int MouseCode::MC_WHEEL_LEFT			= 32;
int MouseCode::MC_WHEEL_RIGHT			= 33;

int KeyCode::KC_UNKNOWN					= SDLK_UNKNOWN;
int KeyCode::KC_RETURN					= SDLK_RETURN;
int KeyCode::KC_ESCAPE					= SDLK_ESCAPE;
int KeyCode::KC_BACKSPACE				= SDLK_BACKSPACE;
int KeyCode::KC_TAB						= SDLK_TAB;
int KeyCode::KC_SPACE					= SDLK_SPACE;
int KeyCode::KC_EXCLAIM					= SDLK_EXCLAIM;
int KeyCode::KC_QUOTEDBL				= SDLK_QUOTEDBL;
int KeyCode::KC_HASH					= SDLK_HASH;
int KeyCode::KC_PERCENT					= SDLK_PERCENT;
int KeyCode::KC_DOLLAR					= SDLK_DOLLAR;
int KeyCode::KC_AMPERSAND				= SDLK_AMPERSAND;
int KeyCode::KC_QUOTE					= SDLK_QUOTE;
int KeyCode::KC_LEFTPAREN				= SDLK_LEFTPAREN;
int KeyCode::KC_RIGHTPAREN				= SDLK_RIGHTPAREN;
int KeyCode::KC_ASTERISK				= SDLK_ASTERISK;
int KeyCode::KC_PLUS					= SDLK_PLUS;
int KeyCode::KC_COMMA					= SDLK_COMMA;
int KeyCode::KC_MINUS					= SDLK_MINUS;
int KeyCode::KC_PERIOD					= SDLK_PERIOD;
int KeyCode::KC_SLASH					= SDLK_SLASH;
int KeyCode::KC_N_0						= SDLK_0;
int KeyCode::KC_N_1						= SDLK_1;
int KeyCode::KC_N_2						= SDLK_2;
int KeyCode::KC_N_3						= SDLK_3;
int KeyCode::KC_N_4						= SDLK_4;
int KeyCode::KC_N_5						= SDLK_5;
int KeyCode::KC_N_6						= SDLK_6;
int KeyCode::KC_N_7						= SDLK_7;
int KeyCode::KC_N_8						= SDLK_8;
int KeyCode::KC_N_9						= SDLK_9;
int KeyCode::KC_COLON					= SDLK_COLON;
int KeyCode::KC_SEMICOLON				= SDLK_SEMICOLON;
int KeyCode::KC_LESS					= SDLK_LESS;
int KeyCode::KC_EQUALS					= SDLK_EQUALS;
int KeyCode::KC_GREATER					= SDLK_GREATER;
int KeyCode::KC_QUESTION				= SDLK_QUESTION;
int KeyCode::KC_AT						= SDLK_AT;
int KeyCode::KC_LEFTBRACKET				= SDLK_LEFTBRACKET;
int KeyCode::KC_BACKSLASH				= SDLK_BACKSLASH;
int KeyCode::KC_RIGHTBRACKET			= SDLK_RIGHTBRACKET;
int KeyCode::KC_CARET					= SDLK_CARET;
int KeyCode::KC_UNDERSCORE				= SDLK_UNDERSCORE;
int KeyCode::KC_BACKQUOTE				= SDLK_BACKQUOTE;
int KeyCode::KC_A						= 'A';
int KeyCode::KC_B						= 'B';
int KeyCode::KC_C						= 'C';
int KeyCode::KC_D						= 'D';
int KeyCode::KC_E						= 'E';
int KeyCode::KC_F						= 'F';
int KeyCode::KC_G						= 'G';
int KeyCode::KC_H						= 'H';
int KeyCode::KC_I						= 'I';
int KeyCode::KC_J						= 'J';
int KeyCode::KC_K						= 'K';
int KeyCode::KC_L						= 'L';
int KeyCode::KC_M						= 'M';
int KeyCode::KC_N						= 'N';
int KeyCode::KC_O						= 'O';
int KeyCode::KC_P						= 'P';
int KeyCode::KC_Q						= 'Q';
int KeyCode::KC_R						= 'R';
int KeyCode::KC_S						= 'S';
int KeyCode::KC_T						= 'T';
int KeyCode::KC_U						= 'U';
int KeyCode::KC_V						= 'V';
int KeyCode::KC_W						= 'W';
int KeyCode::KC_X						= 'X';
int KeyCode::KC_Y						= 'Y';
int KeyCode::KC_Z						= 'Z';
int KeyCode::KC_CAPSLOCK				= SDLK_CAPSLOCK;
int KeyCode::KC_F1						= SDLK_F1;
int KeyCode::KC_F2						= SDLK_F2;
int KeyCode::KC_F3						= SDLK_F3;
int KeyCode::KC_F4						= SDLK_F4;
int KeyCode::KC_F5						= SDLK_F5;
int KeyCode::KC_F6						= SDLK_F6;
int KeyCode::KC_F7						= SDLK_F7;
int KeyCode::KC_F8						= SDLK_F8;
int KeyCode::KC_F9						= SDLK_F9;
int KeyCode::KC_F10						= SDLK_F10;
int KeyCode::KC_F11						= SDLK_F11;
int KeyCode::KC_F12						= SDLK_F12;
int KeyCode::KC_PRINTSCREEN				= SDLK_PRINTSCREEN;
int KeyCode::KC_SCROLLLOCK				= SDLK_SCROLLLOCK;
int KeyCode::KC_PAUSE					= SDLK_PAUSE;
int KeyCode::KC_INSERT					= SDLK_INSERT;
int KeyCode::KC_HOME					= SDLK_HOME;
int KeyCode::KC_PAGEUP					= SDLK_PAGEUP;
int KeyCode::KC_DELETE					= SDLK_DELETE;
int KeyCode::KC_END						= SDLK_END;
int KeyCode::KC_PAGEDOWN				= SDLK_PAGEDOWN;
int KeyCode::KC_RIGHT					= SDLK_RIGHT;
int KeyCode::KC_LEFT					= SDLK_LEFT;
int KeyCode::KC_DOWN					= SDLK_DOWN;
int KeyCode::KC_UP						= SDLK_UP;
int KeyCode::KC_NUMLOCKCLEAR			= SDLK_NUMLOCKCLEAR;
int KeyCode::KC_KP_DIVIDE				= SDLK_KP_DIVIDE;
int KeyCode::KC_KP_MULTIPLY				= SDLK_KP_MULTIPLY;
int KeyCode::KC_KP_MINUS				= SDLK_KP_MINUS;
int KeyCode::KC_KP_PLUS					= SDLK_KP_PLUS;
int KeyCode::KC_KP_ENTER				= SDLK_KP_ENTER;
int KeyCode::KC_KP_1					= SDLK_KP_1;
int KeyCode::KC_KP_2					= SDLK_KP_2;
int KeyCode::KC_KP_3					= SDLK_KP_3;
int KeyCode::KC_KP_4					= SDLK_KP_4;
int KeyCode::KC_KP_5					= SDLK_KP_5;
int KeyCode::KC_KP_6					= SDLK_KP_6;
int KeyCode::KC_KP_7					= SDLK_KP_7;
int KeyCode::KC_KP_8					= SDLK_KP_8;
int KeyCode::KC_KP_9					= SDLK_KP_9;
int KeyCode::KC_KP_0					= SDLK_KP_0;
int KeyCode::KC_KP_PERIOD				= SDLK_KP_PERIOD;
int KeyCode::KC_APPLICATION				= SDLK_APPLICATION;
int KeyCode::KC_POWER					= SDLK_POWER;
int KeyCode::KC_KP_EQUALS				= SDLK_KP_EQUALS;
int KeyCode::KC_F13						= SDLK_F13;
int KeyCode::KC_F14						= SDLK_F14;
int KeyCode::KC_F15						= SDLK_F15;
int KeyCode::KC_F16						= SDLK_F16;
int KeyCode::KC_F17						= SDLK_F17;
int KeyCode::KC_F18						= SDLK_F18;
int KeyCode::KC_F19						= SDLK_F19;
int KeyCode::KC_F20						= SDLK_F20;
int KeyCode::KC_F21						= SDLK_F21;
int KeyCode::KC_F22						= SDLK_F22;
int KeyCode::KC_F23						= SDLK_F23;
int KeyCode::KC_F24						= SDLK_F24;
int KeyCode::KC_EXECUTE					= SDLK_EXECUTE;
int KeyCode::KC_HELP					= SDLK_HELP;
int KeyCode::KC_MENU					= SDLK_MENU;
int KeyCode::KC_SELECT					= SDLK_SELECT;
int KeyCode::KC_STOP					= SDLK_STOP;
int KeyCode::KC_AGAIN					= SDLK_AGAIN;
int KeyCode::KC_UNDO					= SDLK_UNDO;
int KeyCode::KC_CUT						= SDLK_CUT;
int KeyCode::KC_COPY					= SDLK_COPY;
int KeyCode::KC_PASTE					= SDLK_PASTE;
int KeyCode::KC_FIND					= SDLK_FIND;
int KeyCode::KC_MUTE					= SDLK_MUTE;
int KeyCode::KC_VOLUMEUP				= SDLK_VOLUMEUP;
int KeyCode::KC_VOLUMEDOWN				= SDLK_VOLUMEDOWN;
int KeyCode::KC_KP_COMMA				= SDLK_KP_COMMA;
int KeyCode::KC_KP_EQUALSAS400			= SDLK_KP_EQUALSAS400;
int KeyCode::KC_ALTERASE				= SDLK_ALTERASE;
int KeyCode::KC_SYSREQ					= SDLK_SYSREQ;
int KeyCode::KC_CANCEL					= SDLK_CANCEL;
int KeyCode::KC_CLEAR					= SDLK_CLEAR;
int KeyCode::KC_PRIOR					= SDLK_PRIOR;
int KeyCode::KC_RETURN2					= SDLK_RETURN2;
int KeyCode::KC_SEPARATOR				= SDLK_SEPARATOR;
int KeyCode::KC_OUT						= SDLK_OUT;
int KeyCode::KC_OPER					= SDLK_OPER;
int KeyCode::KC_CLEARAGAIN				= SDLK_CLEARAGAIN;
int KeyCode::KC_CRSEL					= SDLK_CRSEL;
int KeyCode::KC_EXSEL					= SDLK_EXSEL;
int KeyCode::KC_KP_00					= SDLK_KP_00;
int KeyCode::KC_KP_000					= SDLK_KP_000;
int KeyCode::KC_THOUSANDSSEPARATOR		= SDLK_THOUSANDSSEPARATOR;
int KeyCode::KC_DECIMALSEPARATOR		= SDLK_DECIMALSEPARATOR;
int KeyCode::KC_CURRENCYUNIT			= SDLK_CURRENCYUNIT;
int KeyCode::KC_CURRENCYSUBUNIT			= SDLK_CURRENCYSUBUNIT;
int KeyCode::KC_KP_LEFTPAREN			= SDLK_KP_LEFTPAREN;
int KeyCode::KC_KP_RIGHTPAREN			= SDLK_KP_RIGHTPAREN;
int KeyCode::KC_KP_LEFTBRACE			= SDLK_KP_LEFTBRACE;
int KeyCode::KC_KP_RIGHTBRACE			= SDLK_KP_RIGHTBRACE;
int KeyCode::KC_KP_TAB					= SDLK_KP_TAB;
int KeyCode::KC_KP_BACKSPACE			= SDLK_KP_BACKSPACE;
int KeyCode::KC_KP_A					= SDLK_KP_A;
int KeyCode::KC_KP_B					= SDLK_KP_B;
int KeyCode::KC_KP_C					= SDLK_KP_C;
int KeyCode::KC_KP_D					= SDLK_KP_D;
int KeyCode::KC_KP_E					= SDLK_KP_E;
int KeyCode::KC_KP_F					= SDLK_KP_F;
int KeyCode::KC_KP_XOR					= SDLK_KP_XOR;
int KeyCode::KC_KP_POWER				= SDLK_KP_POWER;
int KeyCode::KC_KP_PERCENT				= SDLK_KP_PERCENT;
int KeyCode::KC_KP_LESS					= SDLK_KP_LESS;
int KeyCode::KC_KP_GREATER				= SDLK_KP_GREATER;
int KeyCode::KC_KP_AMPERSAND			= SDLK_KP_AMPERSAND;
int KeyCode::KC_KP_DBLAMPERSAND			= SDLK_KP_DBLAMPERSAND;
int KeyCode::KC_KP_VERTICALBAR			= SDLK_KP_VERTICALBAR;
int KeyCode::KC_KP_DBLVERTICALBAR		= SDLK_KP_DBLVERTICALBAR;
int KeyCode::KC_KP_COLON				= SDLK_KP_COLON;
int KeyCode::KC_KP_HASH					= SDLK_KP_HASH;
int KeyCode::KC_KP_SPACE				= SDLK_KP_SPACE;
int KeyCode::KC_KP_AT					= SDLK_KP_AT;
int KeyCode::KC_KP_EXCLAM				= SDLK_KP_EXCLAM;
int KeyCode::KC_KP_MEMSTORE				= SDLK_KP_MEMSTORE;
int KeyCode::KC_KP_MEMRECALL			= SDLK_KP_MEMRECALL;
int KeyCode::KC_KP_MEMCLEAR				= SDLK_KP_MEMCLEAR;
int KeyCode::KC_KP_MEMADD				= SDLK_KP_MEMADD;
int KeyCode::KC_KP_MEMSUBTRACT			= SDLK_KP_MEMSUBTRACT;
int KeyCode::KC_KP_MEMMULTIPLY			= SDLK_KP_MEMMULTIPLY;
int KeyCode::KC_KP_MEMDIVIDE			= SDLK_KP_MEMDIVIDE;
int KeyCode::KC_KP_PLUSMINUS			= SDLK_KP_PLUSMINUS;
int KeyCode::KC_KP_CLEAR				= SDLK_KP_CLEAR;
int KeyCode::KC_KP_CLEARENTRY			= SDLK_KP_CLEARENTRY;
int KeyCode::KC_KP_BINARY				= SDLK_KP_BINARY;
int KeyCode::KC_KP_OCTAL				= SDLK_KP_OCTAL;
int KeyCode::KC_KP_DECIMAL				= SDLK_KP_DECIMAL;
int KeyCode::KC_KP_HEXADECIMAL			= SDLK_KP_HEXADECIMAL;
int KeyCode::KC_LCTRL					= SDLK_LCTRL;
int KeyCode::KC_LSHIFT					= SDLK_LSHIFT;
int KeyCode::KC_LALT					= SDLK_LALT;
int KeyCode::KC_LGUI					= SDLK_LGUI;
int KeyCode::KC_RCTRL					= SDLK_RCTRL;
int KeyCode::KC_RSHIFT					= SDLK_RSHIFT;
int KeyCode::KC_RALT					= SDLK_RALT;
int KeyCode::KC_RGUI					= SDLK_RGUI;
int KeyCode::KC_MODE					= SDLK_MODE;
int KeyCode::KC_AUDIONEXT				= SDLK_AUDIONEXT;
int KeyCode::KC_AUDIOPREV				= SDLK_AUDIOPREV;
int KeyCode::KC_AUDIOSTOP				= SDLK_AUDIOSTOP;
int KeyCode::KC_AUDIOPLAY				= SDLK_AUDIOPLAY;
int KeyCode::KC_AUDIOMUTE				= SDLK_AUDIOMUTE;
int KeyCode::KC_MEDIASELECT				= SDLK_MEDIASELECT;
int KeyCode::KC_WWW						= SDLK_WWW;
int KeyCode::KC_MAIL					= SDLK_MAIL;
int KeyCode::KC_CALCULATOR				= SDLK_CALCULATOR;
int KeyCode::KC_COMPUTER				= SDLK_COMPUTER;
int KeyCode::KC_AC_SEARCH				= SDLK_AC_SEARCH;
int KeyCode::KC_AC_HOME					= SDLK_AC_HOME;
int KeyCode::KC_AC_BACK					= SDLK_AC_BACK;
int KeyCode::KC_AC_FORWARD				= SDLK_AC_FORWARD;
int KeyCode::KC_AC_STOP					= SDLK_AC_STOP;
int KeyCode::KC_AC_REFRESH				= SDLK_AC_REFRESH;
int KeyCode::KC_AC_BOOKMARKS			= SDLK_AC_BOOKMARKS;
int KeyCode::KC_BRIGHTNESSDOWN			= SDLK_BRIGHTNESSDOWN;
int KeyCode::KC_BRIGHTNESSUP			= SDLK_BRIGHTNESSUP;
int KeyCode::KC_DISPLAYSWITCH			= SDLK_DISPLAYSWITCH;
int KeyCode::KC_KBDILLUMTOGGLE			= SDLK_KBDILLUMTOGGLE;
int KeyCode::KC_KBDILLUMDOWN			= SDLK_KBDILLUMDOWN;
int KeyCode::KC_KBDILLUMUP				= SDLK_KBDILLUMUP;
int KeyCode::KC_EJECT					= SDLK_EJECT;
int KeyCode::KC_SLEEP					= SDLK_SLEEP;
int KeyCode::KC_APP1					= SDLK_APP1;
int KeyCode::KC_APP2					= SDLK_APP2;
int KeyCode::KC_AUDIOREWIND				= SDLK_AUDIOREWIND;
int KeyCode::KC_AUDIOFASTFORWARD		= SDLK_AUDIOFASTFORWARD;

std::ostream& operator<<(std::ostream& os, const InputState& state)
{
	switch (state)
	{
	case InputState::NONE:
		os << "NONE";
		break;
	case InputState::RELEASED:
		os << "RELEASED";
		break;
	case InputState::PRESSED:
		os << "PRESSED";
		break;
	case InputState::HOLDING:
		os << "HOLDING";
		break;
	default:
		os << "UNKNOWN";
		break;
	}
	return os;
}
