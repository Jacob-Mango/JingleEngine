#include "Binding.h"

#include "Application.h"

#include <imgui.h>

void Binding::UpdateState()
{
	BindingState previousState = m_State;

	m_State = m_Combo.size() > 0 ? BindingState::PRESSED : BindingState::NONE;

	for (auto& [key, value] : m_Combo)
	{
		if (value.second == InputType::MOUSE && (key == MouseCode::WHEEL_UP || key == MouseCode::WHEEL_DOWN || key == MouseCode::WHEEL_LEFT || key == MouseCode::WHEEL_RIGHT))
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

void Binding::UpdateKey(int keyCode, BindingState state, InputType type, float val)
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

void BindingManager::Init()
{
	Application::Get()->OnKeyPress += OnKeyPress;
	Application::Get()->OnKeyRelease += OnKeyRelease;
	Application::Get()->OnMouseButtonPress += OnMouseButtonPress;
	Application::Get()->OnMouseButtonRelease += OnMouseButtonRelease;
	Application::Get()->OnMouseScroll += OnMouseScroll;
	Application::Get()->OnMouseMove += OnMouseMove;
}

void BindingManager::Destroy()
{
}

void BindingManager::OnKeyPress(BaseClass* sender, const KeyPressEventArgs& args)
{
	for (const auto& [key, value] : m_Bindings)
		for (auto binding : value)
			binding->UpdateKey(args.Key, BindingState::PRESSED, InputType::KEY, 1.0f);
}

void BindingManager::OnKeyRelease(BaseClass* sender, const KeyReleaseEventArgs& args)
{
	for (const auto& [key, value] : m_Bindings)
		for (auto binding : value)
			binding->UpdateKey(args.Key, BindingState::RELEASED, InputType::KEY, 0.0f);
}

void BindingManager::OnMouseButtonPress(BaseClass* sender, const MouseButtonPressEventArgs& args)
{
	for (const auto& [key, value] : m_Bindings)
		for (auto binding : value)
			binding->UpdateKey(args.Button, BindingState::PRESSED, InputType::MOUSE, 1.0f);
}

void BindingManager::OnMouseButtonRelease(BaseClass* sender, const MouseButtonReleaseEventArgs& args)
{
	for (const auto& [key, value] : m_Bindings)
		for (auto binding : value)
			binding->UpdateKey(args.Button, BindingState::RELEASED, InputType::MOUSE, 0.0f);
}

void BindingManager::OnMouseScroll(BaseClass* sender, const MouseScrollEventArgs& args)
{
	float mul = args.Direction == 1 ? -1 : 1;
	float scrollValue = 0;
	int keyCode = 0;

	if (args.Y != 0)
	{
		scrollValue = args.Y * mul;
		keyCode = MouseCode::WHEEL_UP;
		if (scrollValue > 0) keyCode = MouseCode::WHEEL_DOWN;
	}

	if (args.X != 0)
	{
		scrollValue = args.X * mul;
		keyCode = MouseCode::WHEEL_LEFT;
		if (scrollValue > 0) keyCode = MouseCode::WHEEL_RIGHT;
	}

	if (keyCode == 0) return;

	for (const auto& [key, value] : m_Bindings)
		for (auto binding : value)
			binding->UpdateKey(keyCode, BindingState::PRESSED, InputType::MOUSE, scrollValue);
}

void BindingManager::OnMouseMove(BaseClass* sender, const MouseMoveEventArgs& args)
{
}

void BindingManager::RegisterCombo(std::string name, std::initializer_list<std::pair<int, InputType >> combo)
{
	Binding* binding = new Binding();
	binding->m_Name = name;
	for (auto key : combo)
		binding->m_Combo[key.first] = { BindingState::RELEASED, key.second };
	m_Bindings[name].push_back(binding);
}

void BindingManager::RegisterCombos(std::string name, std::initializer_list<std::initializer_list<std::pair<int, InputType>>> combos)
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

void BindingManager::Update()
{
	for (const auto& [key, value] : m_Bindings)
		for (auto binding : value)
			binding->UpdateState();
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
