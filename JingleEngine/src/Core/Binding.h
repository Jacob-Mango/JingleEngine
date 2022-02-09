#pragma once

#include <string>
#include <vector>
#include <map>

#include "Input.h"
#include "Event.h"

enum class BindingState
{
	NONE = 0b0000,
	RELEASED = 0b0001,
	PRESSED = 0b0010,
	HOLDING = 0b0100,
};

class BindingManager;

class Binding
{
	friend class BindingManager;

private:
	std::string m_Name = "";

	std::map<int, std::pair<BindingState, InputType>> m_Combo;

	BindingState m_State = BindingState::NONE;
	float m_Value = 0;

public:
	void UpdateState();

	void UpdateKey(int keyCode, BindingState state, InputType type, float value);
};

class BindingManager
{
private:
	static std::map<std::string, std::vector<Binding*>> m_Bindings;

public:
	static void Init();
	static void Destroy();

private:
	static void OnKeyPress(BaseClass* sender, const KeyPressEventArgs& args);
	static void OnKeyRelease(BaseClass* sender, const KeyReleaseEventArgs& args);
	static void OnMouseButtonPress(BaseClass* sender, const MouseButtonPressEventArgs& args);
	static void OnMouseButtonRelease(BaseClass* sender, const MouseButtonReleaseEventArgs& args);
	static void OnMouseScroll(BaseClass* sender, const MouseScrollEventArgs& args);
	static void OnMouseMove(BaseClass* sender, const MouseMoveEventArgs& args);

public:
	static void RegisterCombo(std::string name, std::initializer_list<std::pair<int, InputType>> combo);

	static void RegisterCombos(std::string name, std::initializer_list<std::initializer_list<std::pair<int, InputType>>> combos);

	static BindingState Get(std::string name);
	static float GetValue(std::string name);

	static void Update();
};

std::ostream& operator<<(std::ostream& os, const BindingState& state);