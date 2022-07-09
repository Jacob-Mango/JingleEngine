#pragma once

#include "Core/Core.h"

#include "Editor/EditorUI.h"

#include "Property/Property.h"

#include <imgui.h>

class ArrayProperty;
class ObjectProperty;

class BaseProperty
{
	friend ArrayProperty;
	friend ObjectProperty;

protected:
	Property* m_VariableProperty;
	JingleScript::Type* m_propType;
	BaseProperty* m_Owner;

	// Is the memory offset the property is at or the index in an array
	int64_t m_Offset;

public:
	BaseProperty();
	BaseProperty(BaseProperty* parent, JingleScript::Type* type, Property* varProperty);
	virtual ~BaseProperty();

	virtual bool OnSerialize(Config* cfg, void*& data) = 0;
	virtual bool OnDeserialize(Config* cfg, void*& data) = 0;

	virtual void Editor_OnRender(void*& data) = 0;
	virtual void Editor_OnPropertyChanged(std::string name) {}

public:
	BaseProperty* GetPropertyOwner() const;
	JingleScript::Type* GetPropertyType() const;
	Property* GetPropertyAttribute() const;
	int64_t GetPropertyOffset() const;

};
