#pragma once

#include "Core/Core.h"

#include "Property/ObjectProperty.h"

class ShaderVariable : public JingleScript::Object, public ObjectProperty
{
	DEFINE_CLASS(ShaderVariable, JingleScript::Object);

public:

public:
	ShaderVariable();
	~ShaderVariable();

};

class ShaderVariableArray : public JingleScript::Array<ShaderVariable*>
{
	DEFINE_CLASS(ShaderVariableArray, JingleScript::Array<ShaderVariable*>);

public:
	ShaderVariableArray() {}
	~ShaderVariableArray() {}

};
