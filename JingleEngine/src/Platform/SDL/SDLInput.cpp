#include "SDLWindow.h"

#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/Window.h"

#include <SDL.h>

void Input::SetMousePosition(int x, int y)
{
	auto window = g_Application->GetWindow()->Cast<SDLWindow>();
	SDL_WarpMouseInWindow(window.m_SDLWindow, x, y);
}

std::pair<int, int> Input::GetMousePosition()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	return { x, y };
}

int g_MouseDeltaX, g_MouseDeltaY;

std::pair<int, int> Input::GetMouseDelta()
{
	return { g_MouseDeltaX, g_MouseDeltaY };
}

void Input::ShowCursor(bool show)
{
	SDL_SetRelativeMouseMode(show ? SDL_FALSE : SDL_TRUE);
}

bool Input::IsCursorVisible()
{
	return SDL_GetRelativeMouseMode() == SDL_FALSE ? true : false;
}

void Input::Update()
{
	SDL_GetRelativeMouseState(&g_MouseDeltaX, &g_MouseDeltaY);
}

int MouseCode::BUTTON_1				= 1;
int MouseCode::BUTTON_2				= 2;
int MouseCode::WHEEL_UP				= 30;
int MouseCode::WHEEL_DOWN			= 31;
int MouseCode::WHEEL_LEFT			= 32;
int MouseCode::WHEEL_RIGHT			= 33;

int KeyCode::UNKNOWN				= SDLK_UNKNOWN;
int KeyCode::RETURN					= SDLK_RETURN;
int KeyCode::ESCAPE					= SDLK_ESCAPE;
int KeyCode::BACKSPACE				= SDLK_BACKSPACE;
int KeyCode::TAB					= SDLK_TAB;
int KeyCode::SPACE					= SDLK_SPACE;
int KeyCode::EXCLAIM				= SDLK_EXCLAIM;
int KeyCode::QUOTEDBL				= SDLK_QUOTEDBL;
int KeyCode::HASH					= SDLK_HASH;
int KeyCode::PERCENT				= SDLK_PERCENT;
int KeyCode::DOLLAR					= SDLK_DOLLAR;
int KeyCode::AMPERSAND				= SDLK_AMPERSAND;
int KeyCode::QUOTE					= SDLK_QUOTE;
int KeyCode::LEFTPAREN				= SDLK_LEFTPAREN;
int KeyCode::RIGHTPAREN				= SDLK_RIGHTPAREN;
int KeyCode::ASTERISK				= SDLK_ASTERISK;
int KeyCode::PLUS					= SDLK_PLUS;
int KeyCode::COMMA					= SDLK_COMMA;
int KeyCode::MINUS					= SDLK_MINUS;
int KeyCode::PERIOD					= SDLK_PERIOD;
int KeyCode::SLASH					= SDLK_SLASH;
int KeyCode::N_0					= SDLK_0;
int KeyCode::N_1					= SDLK_1;
int KeyCode::N_2					= SDLK_2;
int KeyCode::N_3					= SDLK_3;
int KeyCode::N_4					= SDLK_4;
int KeyCode::N_5					= SDLK_5;
int KeyCode::N_6					= SDLK_6;
int KeyCode::N_7					= SDLK_7;
int KeyCode::N_8					= SDLK_8;
int KeyCode::N_9					= SDLK_9;
int KeyCode::COLON					= SDLK_COLON;
int KeyCode::SEMICOLON				= SDLK_SEMICOLON;
int KeyCode::LESS					= SDLK_LESS;
int KeyCode::EQUALS					= SDLK_EQUALS;
int KeyCode::GREATER				= SDLK_GREATER;
int KeyCode::QUESTION				= SDLK_QUESTION;
int KeyCode::AT						= SDLK_AT;
int KeyCode::LEFTBRACKET			= SDLK_LEFTBRACKET;
int KeyCode::BACKSLASH				= SDLK_BACKSLASH;
int KeyCode::RIGHTBRACKET			= SDLK_RIGHTBRACKET;
int KeyCode::CARET					= SDLK_CARET;
int KeyCode::UNDERSCORE				= SDLK_UNDERSCORE;
int KeyCode::BACKQUOTE				= SDLK_BACKQUOTE;
int KeyCode::A						= SDLK_a;
int KeyCode::B						= SDLK_b;
int KeyCode::C						= SDLK_c;
int KeyCode::D						= SDLK_d;
int KeyCode::E						= SDLK_e;
int KeyCode::F						= SDLK_f;
int KeyCode::G						= SDLK_g;
int KeyCode::H						= SDLK_h;
int KeyCode::I						= SDLK_i;
int KeyCode::J						= SDLK_j;
int KeyCode::K						= SDLK_k;
int KeyCode::L						= SDLK_l;
int KeyCode::M						= SDLK_m;
int KeyCode::N						= SDLK_n;
int KeyCode::O						= SDLK_o;
int KeyCode::P						= SDLK_p;
int KeyCode::Q						= SDLK_q;
int KeyCode::R						= SDLK_r;
int KeyCode::S						= SDLK_s;
int KeyCode::T						= SDLK_t;
int KeyCode::U						= SDLK_u;
int KeyCode::V						= SDLK_v;
int KeyCode::W						= SDLK_w;
int KeyCode::X						= SDLK_x;
int KeyCode::Y						= SDLK_y;
int KeyCode::Z						= SDLK_z;
int KeyCode::CAPSLOCK				= SDLK_CAPSLOCK;
int KeyCode::F1						= SDLK_F1;
int KeyCode::F2						= SDLK_F2;
int KeyCode::F3						= SDLK_F3;
int KeyCode::F4						= SDLK_F4;
int KeyCode::F5						= SDLK_F5;
int KeyCode::F6						= SDLK_F6;
int KeyCode::F7						= SDLK_F7;
int KeyCode::F8						= SDLK_F8;
int KeyCode::F9						= SDLK_F9;
int KeyCode::F10					= SDLK_F10;
int KeyCode::F11					= SDLK_F11;
int KeyCode::F12					= SDLK_F12;
int KeyCode::PRINTSCREEN			= SDLK_PRINTSCREEN;
int KeyCode::SCROLLLOCK				= SDLK_SCROLLLOCK;
int KeyCode::PAUSE					= SDLK_PAUSE;
int KeyCode::INSERT					= SDLK_INSERT;
int KeyCode::HOME					= SDLK_HOME;
int KeyCode::PAGEUP					= SDLK_PAGEUP;
int KeyCode::DELETE					= SDLK_DELETE;
int KeyCode::END					= SDLK_END;
int KeyCode::PAGEDOWN				= SDLK_PAGEDOWN;
int KeyCode::RIGHT					= SDLK_RIGHT;
int KeyCode::LEFT					= SDLK_LEFT;
int KeyCode::DOWN					= SDLK_DOWN;
int KeyCode::UP						= SDLK_UP;
int KeyCode::NUMLOCKCLEAR			= SDLK_NUMLOCKCLEAR;
int KeyCode::KP_DIVIDE				= SDLK_KP_DIVIDE;
int KeyCode::KP_MULTIPLY			= SDLK_KP_MULTIPLY;
int KeyCode::KP_MINUS				= SDLK_KP_MINUS;
int KeyCode::KP_PLUS				= SDLK_KP_PLUS;
int KeyCode::KP_ENTER				= SDLK_KP_ENTER;
int KeyCode::KP_1					= SDLK_KP_1;
int KeyCode::KP_2					= SDLK_KP_2;
int KeyCode::KP_3					= SDLK_KP_3;
int KeyCode::KP_4					= SDLK_KP_4;
int KeyCode::KP_5					= SDLK_KP_5;
int KeyCode::KP_6					= SDLK_KP_6;
int KeyCode::KP_7					= SDLK_KP_7;
int KeyCode::KP_8					= SDLK_KP_8;
int KeyCode::KP_9					= SDLK_KP_9;
int KeyCode::KP_0					= SDLK_KP_0;
int KeyCode::KP_PERIOD				= SDLK_KP_PERIOD;
int KeyCode::APPLICATION			= SDLK_APPLICATION;
int KeyCode::POWER					= SDLK_POWER;
int KeyCode::KP_EQUALS				= SDLK_KP_EQUALS;
int KeyCode::F13					= SDLK_F13;
int KeyCode::F14					= SDLK_F14;
int KeyCode::F15					= SDLK_F15;
int KeyCode::F16					= SDLK_F16;
int KeyCode::F17					= SDLK_F17;
int KeyCode::F18					= SDLK_F18;
int KeyCode::F19					= SDLK_F19;
int KeyCode::F20					= SDLK_F20;
int KeyCode::F21					= SDLK_F21;
int KeyCode::F22					= SDLK_F22;
int KeyCode::F23					= SDLK_F23;
int KeyCode::F24					= SDLK_F24;
int KeyCode::EXECUTE				= SDLK_EXECUTE;
int KeyCode::HELP					= SDLK_HELP;
int KeyCode::MENU					= SDLK_MENU;
int KeyCode::SELECT					= SDLK_SELECT;
int KeyCode::STOP					= SDLK_STOP;
int KeyCode::AGAIN					= SDLK_AGAIN;
int KeyCode::UNDO					= SDLK_UNDO;
int KeyCode::CUT					= SDLK_CUT;
int KeyCode::COPY					= SDLK_COPY;
int KeyCode::PASTE					= SDLK_PASTE;
int KeyCode::FIND					= SDLK_FIND;
int KeyCode::MUTE					= SDLK_MUTE;
int KeyCode::VOLUMEUP				= SDLK_VOLUMEUP;
int KeyCode::VOLUMEDOWN				= SDLK_VOLUMEDOWN;
int KeyCode::KP_COMMA				= SDLK_KP_COMMA;
int KeyCode::KP_EQUALSAS400			= SDLK_KP_EQUALSAS400;
int KeyCode::ALTERASE				= SDLK_ALTERASE;
int KeyCode::SYSREQ					= SDLK_SYSREQ;
int KeyCode::CANCEL					= SDLK_CANCEL;
int KeyCode::CLEAR					= SDLK_CLEAR;
int KeyCode::PRIOR					= SDLK_PRIOR;
int KeyCode::RETURN2				= SDLK_RETURN2;
int KeyCode::SEPARATOR				= SDLK_SEPARATOR;
int KeyCode::OUT					= SDLK_OUT;
int KeyCode::OPER					= SDLK_OPER;
int KeyCode::CLEARAGAIN				= SDLK_CLEARAGAIN;
int KeyCode::CRSEL					= SDLK_CRSEL;
int KeyCode::EXSEL					= SDLK_EXSEL;
int KeyCode::KP_00					= SDLK_KP_00;
int KeyCode::KP_000					= SDLK_KP_000;
int KeyCode::THOUSANDSSEPARATOR		= SDLK_THOUSANDSSEPARATOR;
int KeyCode::DECIMALSEPARATOR		= SDLK_DECIMALSEPARATOR;
int KeyCode::CURRENCYUNIT			= SDLK_CURRENCYUNIT;
int KeyCode::CURRENCYSUBUNIT		= SDLK_CURRENCYSUBUNIT;
int KeyCode::KP_LEFTPAREN			= SDLK_KP_LEFTPAREN;
int KeyCode::KP_RIGHTPAREN			= SDLK_KP_RIGHTPAREN;
int KeyCode::KP_LEFTBRACE			= SDLK_KP_LEFTBRACE;
int KeyCode::KP_RIGHTBRACE			= SDLK_KP_RIGHTBRACE;
int KeyCode::KP_TAB					= SDLK_KP_TAB;
int KeyCode::KP_BACKSPACE			= SDLK_KP_BACKSPACE;
int KeyCode::KP_A					= SDLK_KP_A;
int KeyCode::KP_B					= SDLK_KP_B;
int KeyCode::KP_C					= SDLK_KP_C;
int KeyCode::KP_D					= SDLK_KP_D;
int KeyCode::KP_E					= SDLK_KP_E;
int KeyCode::KP_F					= SDLK_KP_F;
int KeyCode::KP_XOR					= SDLK_KP_XOR;
int KeyCode::KP_POWER				= SDLK_KP_POWER;
int KeyCode::KP_PERCENT				= SDLK_KP_PERCENT;
int KeyCode::KP_LESS				= SDLK_KP_LESS;
int KeyCode::KP_GREATER				= SDLK_KP_GREATER;
int KeyCode::KP_AMPERSAND			= SDLK_KP_AMPERSAND;
int KeyCode::KP_DBLAMPERSAND		= SDLK_KP_DBLAMPERSAND;
int KeyCode::KP_VERTICALBAR			= SDLK_KP_VERTICALBAR;
int KeyCode::KP_DBLVERTICALBAR		= SDLK_KP_DBLVERTICALBAR;
int KeyCode::KP_COLON				= SDLK_KP_COLON;
int KeyCode::KP_HASH				= SDLK_KP_HASH;
int KeyCode::KP_SPACE				= SDLK_KP_SPACE;
int KeyCode::KP_AT					= SDLK_KP_AT;
int KeyCode::KP_EXCLAM				= SDLK_KP_EXCLAM;
int KeyCode::KP_MEMSTORE			= SDLK_KP_MEMSTORE;
int KeyCode::KP_MEMRECALL			= SDLK_KP_MEMRECALL;
int KeyCode::KP_MEMCLEAR			= SDLK_KP_MEMCLEAR;
int KeyCode::KP_MEMADD				= SDLK_KP_MEMADD;
int KeyCode::KP_MEMSUBTRACT			= SDLK_KP_MEMSUBTRACT;
int KeyCode::KP_MEMMULTIPLY			= SDLK_KP_MEMMULTIPLY;
int KeyCode::KP_MEMDIVIDE			= SDLK_KP_MEMDIVIDE;
int KeyCode::KP_PLUSMINUS			= SDLK_KP_PLUSMINUS;
int KeyCode::KP_CLEAR				= SDLK_KP_CLEAR;
int KeyCode::KP_CLEARENTRY			= SDLK_KP_CLEARENTRY;
int KeyCode::KP_BINARY				= SDLK_KP_BINARY;
int KeyCode::KP_OCTAL				= SDLK_KP_OCTAL;
int KeyCode::KP_DECIMAL				= SDLK_KP_DECIMAL;
int KeyCode::KP_HEXADECIMAL			= SDLK_KP_HEXADECIMAL;
int KeyCode::LCTRL					= SDLK_LCTRL;
int KeyCode::LSHIFT					= SDLK_LSHIFT;
int KeyCode::LALT					= SDLK_LALT;
int KeyCode::LGUI					= SDLK_LGUI;
int KeyCode::RCTRL					= SDLK_RCTRL;
int KeyCode::RSHIFT					= SDLK_RSHIFT;
int KeyCode::RALT					= SDLK_RALT;
int KeyCode::RGUI					= SDLK_RGUI;
int KeyCode::MODE					= SDLK_MODE;
int KeyCode::AUDIONEXT				= SDLK_AUDIONEXT;
int KeyCode::AUDIOPREV				= SDLK_AUDIOPREV;
int KeyCode::AUDIOSTOP				= SDLK_AUDIOSTOP;
int KeyCode::AUDIOPLAY				= SDLK_AUDIOPLAY;
int KeyCode::AUDIOMUTE				= SDLK_AUDIOMUTE;
int KeyCode::MEDIASELECT			= SDLK_MEDIASELECT;
int KeyCode::WWW					= SDLK_WWW;
int KeyCode::MAIL					= SDLK_MAIL;
int KeyCode::CALCULATOR				= SDLK_CALCULATOR;
int KeyCode::COMPUTER				= SDLK_COMPUTER;
int KeyCode::AC_SEARCH				= SDLK_AC_SEARCH;
int KeyCode::AC_HOME				= SDLK_AC_HOME;
int KeyCode::AC_BACK				= SDLK_AC_BACK;
int KeyCode::AC_FORWARD				= SDLK_AC_FORWARD;
int KeyCode::AC_STOP				= SDLK_AC_STOP;
int KeyCode::AC_REFRESH				= SDLK_AC_REFRESH;
int KeyCode::AC_BOOKMARKS			= SDLK_AC_BOOKMARKS;
int KeyCode::BRIGHTNESSDOWN			= SDLK_BRIGHTNESSDOWN;
int KeyCode::BRIGHTNESSUP			= SDLK_BRIGHTNESSUP;
int KeyCode::DISPLAYSWITCH			= SDLK_DISPLAYSWITCH;
int KeyCode::KBDILLUMTOGGLE			= SDLK_KBDILLUMTOGGLE;
int KeyCode::KBDILLUMDOWN			= SDLK_KBDILLUMDOWN;
int KeyCode::KBDILLUMUP				= SDLK_KBDILLUMUP;
int KeyCode::EJECT					= SDLK_EJECT;
int KeyCode::SLEEP					= SDLK_SLEEP;
int KeyCode::APP1					= SDLK_APP1;
int KeyCode::APP2					= SDLK_APP2;
int KeyCode::AUDIOREWIND			= SDLK_AUDIOREWIND;
int KeyCode::AUDIOFASTFORWARD		= SDLK_AUDIOFASTFORWARD;
