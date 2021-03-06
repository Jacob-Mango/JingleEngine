#pragma once

#include "Property/BaseProperty.h"

class AssetProperty : public BaseProperty
{
public:
	AssetProperty(BaseProperty* parent, JingleScript::Type* type, Property* varProperty) : BaseProperty(parent, type, varProperty)
	{ 
		JS_TRACE(Tracers::Property);
	}

	virtual bool OnSerialize(Config* cfg, void*& data) override;
	virtual bool OnDeserialize(Config* cfg, void*& data) override;

	virtual void Editor_OnRender(void*& data) override;

};
