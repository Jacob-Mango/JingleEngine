#pragma once

#include "Core/Module.h"

#include "Input/Input.h"

class Binding;

class BindingModule : public Module
{
	DEFINE_MODULE(BindingModule)

private:
	std::unordered_map<std::string, Binding*> m_Bindings;

public:
	virtual void OnPreInitialize() override;
	virtual void OnDestroy() override;

public:
	void RegisterCombo(std::string name, std::initializer_list<std::pair<int, InputType>> combo);
	void RegisterCombos(std::string name, std::initializer_list<std::initializer_list<std::pair<int, InputType>>> combos);

	Binding* GetByName(std::string name);

	virtual void OnTick(double DeltaTime);
};
