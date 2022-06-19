#pragma once

#include "Core/Core.h"
#include "Core/Event.h"

#include "Input/Input.h"

#include <string>
#include <vector>
#include <map>

class BindingModule;
class Binding;

class BindingEntry
{
	friend class Binding;

private:
	std::map<InputCode, std::pair<InputState, InputType>> m_Combo;

	InputState m_State = InputState::NONE;
	float m_Value = 0;
	Binding* m_Binding;

private:
	void UpdateState();
	void UpdateKey(InputCode keyCode, InputState state, InputType type, float value);

};

class Binding : public BaseClass
{
	friend class BindingModule;

private:
	std::string m_Name = "";

	std::vector<BindingEntry*> m_Entries;

public:
	std::string GetName() const;

	float GetValue() const;
	InputState GetState() const;

	std::string ToString() const;

private:
	Binding(std::string name);

	inline void UpdateState()
	{
		for (auto entry : m_Entries)
		{
			entry->UpdateState();
		}
	}

	inline void RegisterCombo(std::initializer_list<std::pair<InputCode, InputType >> combo)
	{
		BindingEntry* entry = new BindingEntry();
		for (auto key : combo)
		{
			entry->m_Combo[key.first] = { InputState::RELEASED, key.second };
		}

		entry->m_Binding = this;
		m_Entries.push_back(entry);
	}

	inline void UpdateKey(InputCode keyCode, InputState state, InputType type, float value)
	{
		for (auto entry : m_Entries)
		{
			entry->UpdateKey(keyCode, state, type, value);
		}
	}

};
