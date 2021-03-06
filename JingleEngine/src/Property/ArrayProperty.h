#pragma once

#include "Property/BaseProperty.h"

class ObjectProperty;

class ArrayProperty : public BaseProperty
{
	friend ObjectProperty;

private:
	BaseProperty* m_PropertyData = nullptr;

public:
	ArrayProperty(BaseProperty* parent, JingleScript::Type* type, Property* varProperty) : BaseProperty(parent, type, varProperty)
	{ 
		JS_TRACE(Tracers::Property);
	}

	//! 'data' must be dynamically cast to Object
	virtual bool OnSerialize(Config* cfg, void*& data) override;
	virtual bool OnDeserialize(Config* cfg, void*& data) override;

	virtual void Editor_OnRender(void*& data) override;

};
