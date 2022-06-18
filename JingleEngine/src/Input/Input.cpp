#include "Input/Input.h"

bool Input::s_CursorInViewport;

std::string Input::StateToString(InputState state)
{
	switch (state)
	{
	case InputState::NONE:
		return "NONE";
	case InputState::RELEASED:
		return "RELEASED";
	case InputState::PRESSED:
		return "PRESSED";
	case InputState::HOLDING:
		return "HOLDING";
	}
	return "INVALID";
}
