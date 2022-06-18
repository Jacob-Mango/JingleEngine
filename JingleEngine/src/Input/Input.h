#pragma once

#include "Core/Core.h"

enum class InputState;

class Input
{
	friend class BindingModule;
	friend class EditorViewportPanel;

	static bool s_CursorInViewport;

public:
	static void SetMousePosition(int x, int y);
	static std::pair<int, int> GetMousePosition();
	static std::pair<int, int> GetMouseDelta();

	static void ShowCursor(bool show);
	static bool IsCursorVisible();

	static std::string StateToString(InputState state);

	//! TODO: hacked in solution currently, only supports one viewport
	static bool IsCursorInViewport() { return s_CursorInViewport; }
	
private:
	static void Update();

	static void SetCursorInViewport(bool in) { s_CursorInViewport = in; };

};

enum class InputState
{
	NONE = 0b0000,
	RELEASED = 0b0001,
	PRESSED = 0b0010,
	HOLDING = 0b0100,
};

enum class InputType
{
	KEY,
	MOUSE
};

struct MouseCode
{
	static int MC_BUTTON_1;
	static int MC_BUTTON_2;
	static int MC_BUTTON_3;
	static int MC_WHEEL_UP;
	static int MC_WHEEL_DOWN;
	static int MC_WHEEL_LEFT;
	static int MC_WHEEL_RIGHT;
};

struct KeyCode
{
	static int KC_UNKNOWN;
	static int KC_RETURN;
	static int KC_ESCAPE;
	static int KC_BACKSPACE;
	static int KC_TAB;
	static int KC_SPACE;
	static int KC_EXCLAIM;
	static int KC_QUOTEDBL;
	static int KC_HASH;
	static int KC_PERCENT;
	static int KC_DOLLAR;
	static int KC_AMPERSAND;
	static int KC_QUOTE;
	static int KC_LEFTPAREN;
	static int KC_RIGHTPAREN;
	static int KC_ASTERISK;
	static int KC_PLUS;
	static int KC_COMMA;
	static int KC_MINUS;
	static int KC_PERIOD;
	static int KC_SLASH;
	static int KC_N_0;
	static int KC_N_1;
	static int KC_N_2;
	static int KC_N_3;
	static int KC_N_4;
	static int KC_N_5;
	static int KC_N_6;
	static int KC_N_7;
	static int KC_N_8;
	static int KC_N_9;
	static int KC_COLON;
	static int KC_SEMICOLON;
	static int KC_LESS;
	static int KC_EQUALS;
	static int KC_GREATER;
	static int KC_QUESTION;
	static int KC_AT;
	static int KC_LEFTBRACKET;
	static int KC_BACKSLASH;
	static int KC_RIGHTBRACKET;
	static int KC_CARET;
	static int KC_UNDERSCORE;
	static int KC_BACKQUOTE;
	static int KC_A;
	static int KC_B;
	static int KC_C;
	static int KC_D;
	static int KC_E;
	static int KC_F;
	static int KC_G;
	static int KC_H;
	static int KC_I;
	static int KC_J;
	static int KC_K;
	static int KC_L;
	static int KC_M;
	static int KC_N;
	static int KC_O;
	static int KC_P;
	static int KC_Q;
	static int KC_R;
	static int KC_S;
	static int KC_T;
	static int KC_U;
	static int KC_V;
	static int KC_W;
	static int KC_X;
	static int KC_Y;
	static int KC_Z;
	static int KC_CAPSLOCK;
	static int KC_F1;
	static int KC_F2;
	static int KC_F3;
	static int KC_F4;
	static int KC_F5;
	static int KC_F6;
	static int KC_F7;
	static int KC_F8;
	static int KC_F9;
	static int KC_F10;
	static int KC_F11;
	static int KC_F12;
	static int KC_PRINTSCREEN;
	static int KC_SCROLLLOCK;
	static int KC_PAUSE;
	static int KC_INSERT;
	static int KC_HOME;
	static int KC_PAGEUP;
	static int KC_DELETE;
	static int KC_END;
	static int KC_PAGEDOWN;
	static int KC_RIGHT;
	static int KC_LEFT;
	static int KC_DOWN;
	static int KC_UP;
	static int KC_NUMLOCKCLEAR;
	static int KC_KP_DIVIDE;
	static int KC_KP_MULTIPLY;
	static int KC_KP_MINUS;
	static int KC_KP_PLUS;
	static int KC_KP_ENTER;
	static int KC_KP_1;
	static int KC_KP_2;
	static int KC_KP_3;
	static int KC_KP_4;
	static int KC_KP_5;
	static int KC_KP_6;
	static int KC_KP_7;
	static int KC_KP_8;
	static int KC_KP_9;
	static int KC_KP_0;
	static int KC_KP_PERIOD;
	static int KC_APPLICATION;
	static int KC_POWER;
	static int KC_KP_EQUALS;
	static int KC_F13;
	static int KC_F14;
	static int KC_F15;
	static int KC_F16;
	static int KC_F17;
	static int KC_F18;
	static int KC_F19;
	static int KC_F20;
	static int KC_F21;
	static int KC_F22;
	static int KC_F23;
	static int KC_F24;
	static int KC_EXECUTE;
	static int KC_HELP;
	static int KC_MENU;
	static int KC_SELECT;
	static int KC_STOP;
	static int KC_AGAIN;
	static int KC_UNDO;
	static int KC_CUT;
	static int KC_COPY;
	static int KC_PASTE;
	static int KC_FIND;
	static int KC_MUTE;
	static int KC_VOLUMEUP;
	static int KC_VOLUMEDOWN;
	static int KC_KP_COMMA;
	static int KC_KP_EQUALSAS400;
	static int KC_ALTERASE;
	static int KC_SYSREQ;
	static int KC_CANCEL;
	static int KC_CLEAR;
	static int KC_PRIOR;
	static int KC_RETURN2;
	static int KC_SEPARATOR;
	static int KC_OUT;
	static int KC_OPER;
	static int KC_CLEARAGAIN;
	static int KC_CRSEL;
	static int KC_EXSEL;
	static int KC_KP_00;
	static int KC_KP_000;
	static int KC_THOUSANDSSEPARATOR;
	static int KC_DECIMALSEPARATOR;
	static int KC_CURRENCYUNIT;
	static int KC_CURRENCYSUBUNIT;
	static int KC_KP_LEFTPAREN;
	static int KC_KP_RIGHTPAREN;
	static int KC_KP_LEFTBRACE;
	static int KC_KP_RIGHTBRACE;
	static int KC_KP_TAB;
	static int KC_KP_BACKSPACE;
	static int KC_KP_A;
	static int KC_KP_B;
	static int KC_KP_C;
	static int KC_KP_D;
	static int KC_KP_E;
	static int KC_KP_F;
	static int KC_KP_XOR;
	static int KC_KP_POWER;
	static int KC_KP_PERCENT;
	static int KC_KP_LESS;
	static int KC_KP_GREATER;
	static int KC_KP_AMPERSAND;
	static int KC_KP_DBLAMPERSAND;
	static int KC_KP_VERTICALBAR;
	static int KC_KP_DBLVERTICALBAR;
	static int KC_KP_COLON;
	static int KC_KP_HASH;
	static int KC_KP_SPACE;
	static int KC_KP_AT;
	static int KC_KP_EXCLAM;
	static int KC_KP_MEMSTORE;
	static int KC_KP_MEMRECALL;
	static int KC_KP_MEMCLEAR;
	static int KC_KP_MEMADD;
	static int KC_KP_MEMSUBTRACT;
	static int KC_KP_MEMMULTIPLY;
	static int KC_KP_MEMDIVIDE;
	static int KC_KP_PLUSMINUS;
	static int KC_KP_CLEAR;
	static int KC_KP_CLEARENTRY;
	static int KC_KP_BINARY;
	static int KC_KP_OCTAL;
	static int KC_KP_DECIMAL;
	static int KC_KP_HEXADECIMAL;
	static int KC_LCTRL;
	static int KC_LSHIFT;
	static int KC_LALT;
	static int KC_LGUI;
	static int KC_RCTRL;
	static int KC_RSHIFT;
	static int KC_RALT;
	static int KC_RGUI;
	static int KC_MODE;
	static int KC_AUDIONEXT;
	static int KC_AUDIOPREV;
	static int KC_AUDIOSTOP;
	static int KC_AUDIOPLAY;
	static int KC_AUDIOMUTE;
	static int KC_MEDIASELECT;
	static int KC_WWW;
	static int KC_MAIL;
	static int KC_CALCULATOR;
	static int KC_COMPUTER;
	static int KC_AC_SEARCH;
	static int KC_AC_HOME;
	static int KC_AC_BACK;
	static int KC_AC_FORWARD;
	static int KC_AC_STOP;
	static int KC_AC_REFRESH;
	static int KC_AC_BOOKMARKS;
	static int KC_BRIGHTNESSDOWN;
	static int KC_BRIGHTNESSUP;
	static int KC_DISPLAYSWITCH;
	static int KC_KBDILLUMTOGGLE;
	static int KC_KBDILLUMDOWN;
	static int KC_KBDILLUMUP;
	static int KC_EJECT;
	static int KC_SLEEP;
	static int KC_APP1;
	static int KC_APP2;
	static int KC_AUDIOREWIND;
	static int KC_AUDIOFASTFORWARD;
};

std::ostream& operator<<(std::ostream& os, const InputState& state);
