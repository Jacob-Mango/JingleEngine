#include "Binding.h"

#include "Application.h"

void Binding::UpdateState()
{
	BindingState previousState = m_State;

	m_State = m_Combo.size() > 0 ? BindingState::PRESSED : BindingState::NONE;

	for (auto& [key, value] : m_Combo)
	{
		if (value.second == BindingType::MOUSE && (key == MOUSE_WHEEL_UP || key == MOUSE_WHEEL_DOWN || key == MOUSE_WHEEL_LEFT || key == MOUSE_WHEEL_RIGHT))
		{
			value.first = BindingState::RELEASED;
			m_State = BindingState::RELEASED;
			previousState = BindingState::RELEASED;
			continue;
		}

		if (value.first == BindingState::RELEASED)
			m_State = BindingState::NONE;
	}

	if (m_State == BindingState::NONE && previousState > BindingState::RELEASED)
		m_State = BindingState::RELEASED;
	else if (m_State == BindingState::PRESSED && previousState >= BindingState::PRESSED)
		m_State = BindingState::HOLDING;

	if (m_State == BindingState::RELEASED || m_State == BindingState::NONE)
	{
		m_State = BindingState::RELEASED;
		m_Value = 0;
	}
}

void Binding::UpdateKey(int keyCode, BindingState state, BindingType type, float val)
{
	for (const auto& [key, value] : m_Combo)
	{
		if (key == keyCode && type == m_Combo[key].second)
		{
			m_Value = val;

			m_Combo[key] = { state, type };
		}
	}
}

std::map<std::string, std::vector<Binding*>> BindingManager::m_Bindings;
bool BindingManager::m_KeyChanged;

int BindingManager::m_PreviousMouseX;
int BindingManager::m_PreviousMouseY;
int BindingManager::m_DeltaMouseX;
int BindingManager::m_DeltaMouseY;
int BindingManager::m_MouseX;
int BindingManager::m_MouseY;
bool BindingManager::m_CalledMouseEvent;

bool BindingManager::m_MouseLock;
Application* BindingManager::m_Application;

void BindingManager::Init(Application* app)
{
	m_Application = app;
}

void BindingManager::Destroy()
{
}

void BindingManager::OnEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		OnKeyboardEventEvent(event.key);
		break;
	case SDL_MOUSEMOTION:
		OnMouseMotionEvent(event.motion);
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		OnMouseButtonEvent(event.button);
		break;
	case SDL_MOUSEWHEEL:
		OnMouseWheelEvent(event.wheel);
		break;
	}
}

void BindingManager::OnKeyboardEventEvent(SDL_KeyboardEvent& event)
{
	m_KeyChanged = (event.state == SDL_PRESSED) ? true : m_KeyChanged;

	for (const auto& [key, value] : m_Bindings)
		for (auto binding : value)
			binding->UpdateKey(event.keysym.sym, event.state ? BindingState::PRESSED : BindingState::RELEASED, BindingType::KEY, event.state ? 1 : 0);
}

void BindingManager::OnMouseMotionEvent(SDL_MouseMotionEvent& event)
{
	if (m_CalledMouseEvent && event.x == m_MouseX && event.y == m_MouseY)
	{
		m_CalledMouseEvent = false;
		return;
	}

	m_PreviousMouseX = m_MouseX;
	m_PreviousMouseY = m_MouseY;

	m_MouseX = event.x;
	m_MouseY = event.y;

	m_DeltaMouseX = m_PreviousMouseX - m_MouseX;
	m_DeltaMouseY = m_PreviousMouseY - m_MouseY;

	if (m_MouseLock)
	{
		auto [width, height] = m_Application->GetSize();
		m_MouseX = width / 2;
		m_MouseY = height / 2;

		m_CalledMouseEvent = true;

		SDL_WarpMouseInWindow(m_Application->GetWindow(), m_MouseX, m_MouseY);
	}
}

void BindingManager::OnMouseButtonEvent(SDL_MouseButtonEvent& event)
{
	for (const auto& [key, value] : m_Bindings)
		for (auto binding : value)
			binding->UpdateKey(event.button, event.state ? BindingState::PRESSED : BindingState::RELEASED, BindingType::MOUSE, event.state ? 1 : 0);
}

void BindingManager::OnMouseWheelEvent(SDL_MouseWheelEvent& event)
{
	Sint32 mul = event.direction == SDL_MOUSEWHEEL_FLIPPED ? -1 : 1;
	float scrollValue = 0;
	int keyCode = 0;

	if (event.y != 0)
	{
		scrollValue = event.y * mul;
		keyCode = MOUSE_WHEEL_UP;
		if (scrollValue > 0) keyCode = MOUSE_WHEEL_DOWN;
	}
	if (event.x != 0)
	{
		scrollValue = event.x * mul;
		keyCode = MOUSE_WHEEL_LEFT;
		if (scrollValue > 0) keyCode = MOUSE_WHEEL_RIGHT;
	}

	if (keyCode == 0) return;

	for (const auto& [key, value] : m_Bindings)
		for (auto binding : value)
			binding->UpdateKey(keyCode, BindingState::PRESSED, BindingType::MOUSE, scrollValue);
}

void BindingManager::RegisterCombo(std::string name, std::initializer_list<std::pair<int, BindingType >> combo)
{
	Binding* binding = new Binding();
	binding->m_Name = name;
	for (auto key : combo)
		binding->m_Combo[key.first] = { BindingState::RELEASED, key.second };
	m_Bindings[name].push_back(binding);
}

void BindingManager::RegisterCombos(std::string name, std::initializer_list<std::initializer_list<std::pair<int, BindingType>>> combos)
{
	for (auto& combo : combos)
		RegisterCombo(name, combo);
}

BindingState BindingManager::Get(std::string name)
{
	BindingState state = BindingState::NONE;
	for (auto binding : m_Bindings[name])
		if (binding->m_State > state)
			state = binding->m_State;
	return state;
}

float BindingManager::GetValue(std::string name)
{
	float value = 0.0f;
	for (auto binding : m_Bindings[name])
		if (binding->m_Value != 0.0f)
			value = binding->m_Value;
	return value;
}

std::pair<int, int> BindingManager::GetMouseDelta()
{
	return { m_DeltaMouseX, m_DeltaMouseY };
}

bool BindingManager::IsMouseLocked()
{
	return m_MouseLock;
}

void BindingManager::MouseLock(bool yes)
{
	m_MouseLock = yes;

	if (m_MouseLock)
	{
		auto [width, height] = m_Application->GetSize();
		m_PreviousMouseX = width / 2;
		m_PreviousMouseY = height / 2;

		SDL_WarpMouseInWindow(m_Application->GetWindow(), m_PreviousMouseX, m_PreviousMouseY);

		m_MouseX = m_PreviousMouseX;
		m_MouseY = m_PreviousMouseY;
	}
}

void BindingManager::Update()
{
	SDL_ShowCursor(m_MouseLock ? SDL_DISABLE : SDL_ENABLE);

	if (m_MouseLock)
	{
		m_DeltaMouseX = 0;
		m_DeltaMouseY = 0;
	}

	for (const auto& [key, value] : m_Bindings)
		for (auto binding : value)
			binding->UpdateState();
}

void BindingManager::ResetKeyChange()
{
	m_KeyChanged = false;
}

bool BindingManager::AnyKeyPressed()
{
	return m_KeyChanged;
}

std::ostream& operator<<(std::ostream& os, const BindingState& state)
{
	switch (state)
	{
	case BindingState::NONE:
		os << "NONE";
		break;
	case BindingState::RELEASED:
		os << "RELEASED";
		break;
	case BindingState::PRESSED:
		os << "PRESSED";
		break;
	case BindingState::HOLDING:
		os << "HOLDING";
		break;
	default:
		os << "UNKNOWN";
		break;
	}
	return os;
}
