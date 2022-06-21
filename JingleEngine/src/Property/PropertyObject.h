#pragma once

#include "Core/Core.h"

#include "Property/PropertyBase.h"

class PropertyObject : public PropertyBase
{
	std::vector<PropertyBase*> m_Properties;
	uint64_t m_Offset;

public:
	PropertyObject();
	PropertyObject(JingleScript::Type* type, Property* property, uint64_t offset);
	virtual ~PropertyObject();

	virtual bool OnDeserialize(Config* cfg) override;
	virtual bool OnSerialize(Config* cfg) override;

	virtual void OnReadObject(JingleScript::Object* instance) override;
	virtual void OnWriteObject(JingleScript::Object* instance) override;

	void ReadObject(JingleScript::Object* instance);
	void WriteObject(JingleScript::Object* instance);

};
