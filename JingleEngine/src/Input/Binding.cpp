#include "Binding.h"

#include "Core/Application.h"

#include <imgui.h>

void BindingEntry::UpdateState()
{
	InputState previousState = m_State;

	m_State = m_Combo.size() > 0 ? InputState::PRESSED : InputState::NONE;

	for (auto& [key, value] : m_Combo)
	{
		if (value.second == InputType::MOUSE && (key == InputCode::MC_WHEEL_UP || key == InputCode::MC_WHEEL_DOWN || key == InputCode::MC_WHEEL_LEFT || key == InputCode::MC_WHEEL_RIGHT))
		{
			value.first = InputState::RELEASED;
			m_State = InputState::RELEASED;
			previousState = InputState::RELEASED;
			continue;
		}

		if (value.first == InputState::RELEASED)
		{
			m_State = InputState::NONE;
		}
	}

	if (m_State == InputState::NONE && previousState > InputState::RELEASED)
	{
		m_State = InputState::RELEASED;
	}
	else if (m_State == InputState::PRESSED && previousState >= InputState::PRESSED)
	{
		m_State = InputState::HOLDING;
	}
	
	if (m_State == InputState::RELEASED || m_State == InputState::NONE)
	{
		m_State = InputState::RELEASED;
		m_Value = 0;
	}
}

void BindingEntry::UpdateKey(InputCode keyCode, InputState state, InputType type, float val)
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

Binding::Binding(std::string name) : m_Name(name)
{

}

std::string Binding::GetName() const
{
	return m_Name;
}

float Binding::GetValue() const
{
	float value = 0.0f;
	for (auto entry : m_Entries)
	{
		if (entry->m_Value != 0.0f)
		{
			value = entry->m_Value;
		}
	}
	return value;
}

InputState Binding::GetState() const
{
	InputState state = InputState::NONE;
	for (auto entry : m_Entries)
	{
		if (entry->m_State > state)
		{
			state = entry->m_State;
		}
	}
	return state;
}

std::string Binding::ToString() const
{
	return GetName() + " {" + Input::StateToString(GetState()) + ", " + std::to_string(GetValue()) + "}";
}
