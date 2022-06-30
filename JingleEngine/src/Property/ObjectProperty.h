#pragma once

#include "Property/BaseProperty.h"

class ObjectProperty : public BaseProperty
{
private:
	std::map<std::string, std::pair<BaseProperty*, bool>> m_Properties;

public:
	ObjectProperty() : BaseProperty()
	{ 
		JS_TRACE(Tracers::Property);
	}
	
	ObjectProperty(BaseProperty* parent, JingleScript::Type* type, Property* varProperty) : BaseProperty(parent, type, varProperty)
	{ 
		JS_TRACE(Tracers::Property);
	}

	//! 'data' must be dynamically cast to Object
	virtual bool OnSerialize(Config* cfg, void*& data) override;
	virtual bool OnDeserialize(Config* cfg, void*& data) override;

	bool Serialize(Config* cfg);
	bool Deserialize(Config* cfg);

};
