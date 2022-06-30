#pragma once

#include "Property/BaseProperty.h"

class StructureProperty : public BaseProperty
{
public:
	StructureProperty(BaseProperty* parent, JingleScript::Type* type, Property* varProperty) : BaseProperty(parent, type, varProperty)
	{ 
		JS_TRACE(Tracers::Property);
	}

	virtual bool OnSerialize(Config* cfg, void*& data) override;
	virtual bool OnDeserialize(Config* cfg, void*& data) override;

	std::string ToString(void*& data);
	bool FromString(std::string& value, void*& data);

	virtual void OnRender(void*& data) override;

};
