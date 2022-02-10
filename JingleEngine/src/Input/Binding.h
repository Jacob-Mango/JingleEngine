#pragma once

#include <string>
#include <vector>
#include <map>

#include "Core/Event.h"
#include "Core/Reference.h"

#include "Input/Input.h"

class BindingModule;

class BindingEntry
{
	friend class Binding;

private:
	std::map<int, std::pair<InputState, InputType>> m_Combo;

	InputState m_State = InputState::NONE;
	float m_Value = 0;

private:
	void UpdateState();
	void UpdateKey(int keyCode, InputState state, InputType type, float value);

};

class Binding : public BaseClass
{
	friend class BindingModule;

private:
	std::string m_Name = "";

	std::vector<BindingEntry*> m_Entries;

public:
	std::string GetName();

	float GetValue();
	InputState GetState();

private:
	Binding(std::string name);

	inline void UpdateState()
	{
		for (auto entry : m_Entries)
			entry->UpdateState();
	}

	inline void RegisterCombo(std::initializer_list<std::pair<int, InputType >> combo)
	{
		BindingEntry* entry = new BindingEntry();
		for (auto key : combo)
			entry->m_Combo[key.first] = { InputState::RELEASED, key.second };
		
		m_Entries.push_back(entry);
	}

	inline void UpdateKey(int keyCode, InputState state, InputType type, float value)
	{
		for (auto entry : m_Entries)
			entry->UpdateKey(keyCode, state, type, value);
	}

};
