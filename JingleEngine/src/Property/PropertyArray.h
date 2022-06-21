#pragma once

#include "Core/Core.h"

#include "Property/PropertyBase.h"

class PropertyArray : public PropertyBase
{
	std::vector<PropertyBase*> m_Properties;
	uint64_t m_Offset;

public:
	PropertyArray(JingleScript::Type* type, Property* property, uint64_t offset);
	virtual ~PropertyArray();

	virtual bool OnDeserialize(Config* cfg) override;
	virtual bool OnSerialize(Config* cfg) override;

	virtual bool OnReadObject(JingleScript::Object* instance) override;
	virtual bool OnWriteObject(JingleScript::Object* instance) override;

	virtual JingleScript::Object* GetWriteInstance(JingleScript::Object* instance) override;

};
