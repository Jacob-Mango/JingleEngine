#pragma once

#include "Core/Core.h"
#include "Core/Event.h"

#include <JingleScript.h>

#define DEFINE_MODULE(type, baseTypeName)					\
	DEFINE_NAMED_CLASS(type, type, baseTypeName)			\

#define DEFINE_VIRTUAL_MODULE(type, baseTypeName)			\
	DEFINE_NAMED_VIRTUAL_CLASS(type, type, baseTypeName)	\

#define BEGIN_MODULE_LINK(type)								\
	BEGIN_CLASS_LINK(type)

#define END_MODULE_LINK()									\
	END_CLASS_LINK()

class Module : public JingleScript::Object
{
	DEFINE_MODULE(Module, JingleScript::Object);

public:
	Module() {}

	virtual void OnPreInitialize();
	virtual void OnInitialize();

	virtual void OnDestroy();

	virtual void OnEvent(BaseClass* sender, const EventArgs& args);
	virtual void OnTick(double DeltaTime);

};

#include "Core/ModuleManager.h"
