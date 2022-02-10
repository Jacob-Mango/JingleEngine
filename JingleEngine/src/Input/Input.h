#pragma once

#include "Core/Core.h"

class Input
{
	friend class BindingModule;

public:
	static void SetMousePosition(int x, int y);
	static std::pair<int, int> GetMousePosition();
	static std::pair<int, int> GetMouseDelta();

	static void ShowCursor(bool show);
	static bool IsCursorVisible();
	
private:
	static void Update();
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
	static int BUTTON_1;
	static int BUTTON_2;
	static int WHEEL_UP;
	static int WHEEL_DOWN;
	static int WHEEL_LEFT;
	static int WHEEL_RIGHT;
};

struct KeyCode
{
	static int UNKNOWN;
	static int RETURN;
	static int ESCAPE;
	static int BACKSPACE;
	static int TAB;
	static int SPACE;
	static int EXCLAIM;
	static int QUOTEDBL;
	static int HASH;
	static int PERCENT;
	static int DOLLAR;
	static int AMPERSAND;
	static int QUOTE;
	static int LEFTPAREN;
	static int RIGHTPAREN;
	static int ASTERISK;
	static int PLUS;
	static int COMMA;
	static int MINUS;
	static int PERIOD;
	static int SLASH;
	static int N_0;
	static int N_1;
	static int N_2;
	static int N_3;
	static int N_4;
	static int N_5;
	static int N_6;
	static int N_7;
	static int N_8;
	static int N_9;
	static int COLON;
	static int SEMICOLON;
	static int LESS;
	static int EQUALS;
	static int GREATER;
	static int QUESTION;
	static int AT;
	static int LEFTBRACKET;
	static int BACKSLASH;
	static int RIGHTBRACKET;
	static int CARET;
	static int UNDERSCORE;
	static int BACKQUOTE;
	static int A;
	static int B;
	static int C;
	static int D;
	static int E;
	static int F;
	static int G;
	static int H;
	static int I;
	static int J;
	static int K;
	static int L;
	static int M;
	static int N;
	static int O;
	static int P;
	static int Q;
	static int R;
	static int S;
	static int T;
	static int U;
	static int V;
	static int W;
	static int X;
	static int Y;
	static int Z;
	static int CAPSLOCK;
	static int F1;
	static int F2;
	static int F3;
	static int F4;
	static int F5;
	static int F6;
	static int F7;
	static int F8;
	static int F9;
	static int F10;
	static int F11;
	static int F12;
	static int PRINTSCREEN;
	static int SCROLLLOCK;
	static int PAUSE;
	static int INSERT;
	static int HOME;
	static int PAGEUP;
	static int DELETE;
	static int END;
	static int PAGEDOWN;
	static int RIGHT;
	static int LEFT;
	static int DOWN;
	static int UP;
	static int NUMLOCKCLEAR;
	static int KP_DIVIDE;
	static int KP_MULTIPLY;
	static int KP_MINUS;
	static int KP_PLUS;
	static int KP_ENTER;
	static int KP_1;
	static int KP_2;
	static int KP_3;
	static int KP_4;
	static int KP_5;
	static int KP_6;
	static int KP_7;
	static int KP_8;
	static int KP_9;
	static int KP_0;
	static int KP_PERIOD;
	static int APPLICATION;
	static int POWER;
	static int KP_EQUALS;
	static int F13;
	static int F14;
	static int F15;
	static int F16;
	static int F17;
	static int F18;
	static int F19;
	static int F20;
	static int F21;
	static int F22;
	static int F23;
	static int F24;
	static int EXECUTE;
	static int HELP;
	static int MENU;
	static int SELECT;
	static int STOP;
	static int AGAIN;
	static int UNDO;
	static int CUT;
	static int COPY;
	static int PASTE;
	static int FIND;
	static int MUTE;
	static int VOLUMEUP;
	static int VOLUMEDOWN;
	static int KP_COMMA;
	static int KP_EQUALSAS400;
	static int ALTERASE;
	static int SYSREQ;
	static int CANCEL;
	static int CLEAR;
	static int PRIOR;
	static int RETURN2;
	static int SEPARATOR;
	static int OUT;
	static int OPER;
	static int CLEARAGAIN;
	static int CRSEL;
	static int EXSEL;
	static int KP_00;
	static int KP_000;
	static int THOUSANDSSEPARATOR;
	static int DECIMALSEPARATOR;
	static int CURRENCYUNIT;
	static int CURRENCYSUBUNIT;
	static int KP_LEFTPAREN;
	static int KP_RIGHTPAREN;
	static int KP_LEFTBRACE;
	static int KP_RIGHTBRACE;
	static int KP_TAB;
	static int KP_BACKSPACE;
	static int KP_A;
	static int KP_B;
	static int KP_C;
	static int KP_D;
	static int KP_E;
	static int KP_F;
	static int KP_XOR;
	static int KP_POWER;
	static int KP_PERCENT;
	static int KP_LESS;
	static int KP_GREATER;
	static int KP_AMPERSAND;
	static int KP_DBLAMPERSAND;
	static int KP_VERTICALBAR;
	static int KP_DBLVERTICALBAR;
	static int KP_COLON;
	static int KP_HASH;
	static int KP_SPACE;
	static int KP_AT;
	static int KP_EXCLAM;
	static int KP_MEMSTORE;
	static int KP_MEMRECALL;
	static int KP_MEMCLEAR;
	static int KP_MEMADD;
	static int KP_MEMSUBTRACT;
	static int KP_MEMMULTIPLY;
	static int KP_MEMDIVIDE;
	static int KP_PLUSMINUS;
	static int KP_CLEAR;
	static int KP_CLEARENTRY;
	static int KP_BINARY;
	static int KP_OCTAL;
	static int KP_DECIMAL;
	static int KP_HEXADECIMAL;
	static int LCTRL;
	static int LSHIFT;
	static int LALT;
	static int LGUI;
	static int RCTRL;
	static int RSHIFT;
	static int RALT;
	static int RGUI;
	static int MODE;
	static int AUDIONEXT;
	static int AUDIOPREV;
	static int AUDIOSTOP;
	static int AUDIOPLAY;
	static int AUDIOMUTE;
	static int MEDIASELECT;
	static int WWW;
	static int MAIL;
	static int CALCULATOR;
	static int COMPUTER;
	static int AC_SEARCH;
	static int AC_HOME;
	static int AC_BACK;
	static int AC_FORWARD;
	static int AC_STOP;
	static int AC_REFRESH;
	static int AC_BOOKMARKS;
	static int BRIGHTNESSDOWN;
	static int BRIGHTNESSUP;
	static int DISPLAYSWITCH;
	static int KBDILLUMTOGGLE;
	static int KBDILLUMDOWN;
	static int KBDILLUMUP;
	static int EJECT;
	static int SLEEP;
	static int APP1;
	static int APP2;
	static int AUDIOREWIND;
	static int AUDIOFASTFORWARD;
};

std::ostream& operator<<(std::ostream& os, const InputState& state);
