#pragma once

#include "Core/Core.h"

#include "Property/PropertyBase.h"

class PropertyObject : public PropertyBase
{
	DEFINE_CLASS(PropertyObject, PropertyBase);

	//! temp
	friend class EntityPropertiesPanel;

	std::map<std::string, PropertyBase*> m_Properties;
	uint64_t m_Offset;

public:
	PropertyObject();
	PropertyObject(JingleScript::Type* type, Property* property, uint64_t offset);
	virtual ~PropertyObject();

	virtual bool OnDeserialize(Config* cfg) override;
	virtual bool OnSerialize(Config* cfg) override;

	virtual bool OnReadObject(JingleScript::Object* instance) override;
	virtual bool OnWriteObject(JingleScript::Object* instance) override;

	virtual JingleScript::Object* GetReadInstance(JingleScript::Object* instance) override;
	virtual JingleScript::Object* GetWriteInstance(JingleScript::Object* instance) override;

};
