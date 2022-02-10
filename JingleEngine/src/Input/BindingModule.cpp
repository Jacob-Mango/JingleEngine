#include "BindingModule.h"

#include "Core/Application.h"

#include "Input/Binding.h"

void BindingModule::OnCreate()
{
	Application::Get()->OnKeyPress += [this](BaseClass* sender, KeyPressEventArgs args)
	{
		for (const auto& [key, binding] : m_Bindings)
			binding->UpdateKey(args.Key, InputState::PRESSED, InputType::KEY, 1.0f);
	};

	Application::Get()->OnKeyRelease += [this](BaseClass* sender, KeyReleaseEventArgs args)
	{
		for (const auto& [key, binding] : m_Bindings)
			binding->UpdateKey(args.Key, InputState::RELEASED, InputType::KEY, 0.0f);
	};

	Application::Get()->OnMouseButtonPress += [this](BaseClass* sender, MouseButtonPressEventArgs args)
	{
		for (const auto& [key, binding] : m_Bindings)
			binding->UpdateKey(args.Button, InputState::PRESSED, InputType::MOUSE, 1.0f);
	};

	Application::Get()->OnMouseButtonRelease += [this](BaseClass* sender, MouseButtonReleaseEventArgs args)
	{
		for (const auto& [key, binding] : m_Bindings)
			binding->UpdateKey(args.Button, InputState::RELEASED, InputType::MOUSE, 0.0f);
	};

	Application::Get()->OnMouseScroll += [this](BaseClass* sender, MouseScrollEventArgs args)
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

		for (const auto& [key, binding] : m_Bindings)
			binding->UpdateKey(keyCode, InputState::PRESSED, InputType::MOUSE, scrollValue);
	};

	Application::Get()->OnMouseMove += [this](BaseClass* sender, MouseMoveEventArgs args)
	{
		
	};
}

void BindingModule::OnDestroy()
{
}

Binding* BindingModule::GetByName(std::string name)
{
	auto it = m_Bindings.find(name);
	if (it == m_Bindings.end())
	{
		return nullptr;
	}

	return it->second;
}

void BindingModule::RegisterCombo(std::string name, std::initializer_list<std::pair<int, InputType >> combo)
{
	auto it = m_Bindings.find(name);
	Binding* binding = it == m_Bindings.end() ? new Binding(name) : it->second;
	if (it == m_Bindings.end())
	{
		m_Bindings[name] = binding;
	}

	binding->RegisterCombo(combo);
}

void BindingModule::RegisterCombos(std::string name, std::initializer_list<std::initializer_list<std::pair<int, InputType>>> combos)
{
	for (auto& combo : combos)
		RegisterCombo(name, combo);
}

void BindingModule::OnTick(double DeltaTime)
{
	Input::Update();
	
	for (const auto& [key, entry] : m_Bindings)
		entry->UpdateState();
}
