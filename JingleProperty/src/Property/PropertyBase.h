#pragma once

#include "Core.h"

#include "Config/Config.h"

#include "Property/Property.h"

class PropertyBase
{
protected:
	JingleScript::Type* m_Type;
	Property* m_Property;

public:
	PropertyBase(JingleScript::Type* type, Property* property);
	virtual ~PropertyBase();

	//! Returns false if deserialization fails
	virtual bool OnDeserialize(Config* cfg) = 0;

	//! Returns false if this property can avoid serialization
	virtual bool OnSerialize(Config* cfg) = 0;

	//! The property is read from the object
	virtual void OnDeserialize(JingleScript::Object* instance) = 0;

	//! The property is written to the object
	virtual void OnSerialize(JingleScript::Object* instance) = 0;

};
