#include "BindingModule.h"

#include "Core/Application.h"
#include "Core/ModuleManager.h"

#include "Input/Binding.h"

BEGIN_MODULE_LINK(BindingModule);
END_MODULE_LINK();

void BindingModule::OnPreInitialize()
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
		float scrollValue = args.Direction;

		InputCode keyCode = InputCode::MC_WHEEL_UP;
		if (scrollValue < 0)
		{
			InputCode keyCode = InputCode::MC_WHEEL_DOWN;
		}

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

void BindingModule::RegisterCombo(std::string name, std::initializer_list<std::pair<InputCode, InputType >> combo)
{
	auto it = m_Bindings.find(name);
	Binding* binding = it == m_Bindings.end() ? new Binding(name) : it->second;
	if (it == m_Bindings.end())
	{
		m_Bindings[name] = binding;
	}

	binding->RegisterCombo(combo);
}

void BindingModule::RegisterCombos(std::string name, std::initializer_list<std::initializer_list<std::pair<InputCode, InputType>>> combos)
{
	for (auto& combo : combos)
	{
		RegisterCombo(name, combo);
	}
}

void BindingModule::Process(double DeltaTime)
{
	for (const auto& [key, entry] : m_Bindings)
	{
		entry->UpdateState();
	}

	if (ImGui::Begin("BindingModule"))
	{
		for (const auto& [key, entry] : m_Bindings)
		{
			ImGui::Text(entry->ToString().c_str());
		}
	}
	ImGui::End();

	Input::Update();
}
