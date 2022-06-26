#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

#include "Property/Property.h"

class PropertyBase : public JingleScript::Object
{
	DEFINE_VIRTUAL_CLASS(PropertyBase, JingleScript::Object);

protected:
	JingleScript::Type* m_PropertyType;
	Property* m_Property;

public:
	PropertyBase(JingleScript::Type* type, Property* property);
	virtual ~PropertyBase();

	//! Returns false if deserialization fails
	virtual bool OnDeserialize(Config* cfg) = 0;

	//! Returns false if this property can avoid serialization
	virtual bool OnSerialize(Config* cfg) = 0;

	//! The property is read from the object
	virtual bool OnReadObject(JingleScript::Object* instance) = 0;

	//! The property is written to the object
	virtual bool OnWriteObject(JingleScript::Object* instance) = 0;

	virtual JingleScript::Object* GetReadInstance(JingleScript::Object* instance) = 0;
	virtual JingleScript::Object* GetWriteInstance(JingleScript::Object* instance) = 0;

	JingleScript::Type* GetPropertyType() const;
	Property* GetPropertyAttribute() const;

};
