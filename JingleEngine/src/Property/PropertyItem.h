#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

#include "Property/PropertyBase.h"

class Property;

class PropertyItem : public PropertyBase
{
	uint64_t m_Offset;
	void* m_Data;

public:
	PropertyItem(JingleScript::Type* type, Property* property, uint64_t offset);
	virtual ~PropertyItem();

	virtual bool OnDeserialize(Config* cfg) override;
	virtual bool OnSerialize(Config* cfg) override;

	virtual void OnReadObject(JingleScript::Object* instance) override;
	virtual void OnWriteObject(JingleScript::Object* instance) override;

};