#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

#include "Property/PropertyBase.h"

class Property;

class PropertyProperty : public PropertyBase
{
	DEFINE_CLASS(PropertyProperty, PropertyBase);

	PropertyBase* m_PropertyData;
	uint64_t m_Offset;

public:
	PropertyProperty(JingleScript::Type* type, Property* property, uint64_t offset);
	virtual ~PropertyProperty();

	virtual bool OnDeserialize(Config* cfg) override;
	virtual bool OnSerialize(Config* cfg) override;

	virtual bool OnReadObject(JingleScript::Object* instance) override;
	virtual bool OnWriteObject(JingleScript::Object* instance) override;

	virtual JingleScript::Object* GetReadInstance(JingleScript::Object* instance) override;
	virtual JingleScript::Object* GetWriteInstance(JingleScript::Object* instance) override;

};
