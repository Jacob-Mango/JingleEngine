#pragma once

#include "Core/Core.h"

#include "Property/PropertyBase.h"

class PropertyObject : public PropertyBase
{
	std::vector<PropertyBase*> m_Properties;
	uint64_t m_Offset;

public:
	PropertyObject(JingleScript::Type* type);
	PropertyObject(JingleScript::Type* type, Property* property, uint64_t offset);
	virtual ~PropertyObject();

	virtual bool OnDeserialize(Config* cfg) override;
	virtual bool OnSerialize(Config* cfg) override;

	virtual void OnDeserialize(JingleScript::Object* instance) override;
	virtual void OnSerialize(JingleScript::Object* instance) override;

	void SerializeToObject(JingleScript::Object* instance);

};
