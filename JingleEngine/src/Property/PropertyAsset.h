#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

#include "Property/PropertyBase.h"

class Property;

class PropertyAsset : public PropertyBase
{
	DEFINE_CLASS(PropertyAsset, PropertyBase);

	uint64_t m_Offset;
	AssetID m_ID;

public:
	PropertyAsset(JingleScript::Type* type, Property* property, uint64_t offset);
	virtual ~PropertyAsset();

	virtual bool OnDeserialize(Config* cfg) override;
	virtual bool OnSerialize(Config* cfg) override;

	virtual bool OnReadObject(JingleScript::Object* instance) override;
	virtual bool OnWriteObject(JingleScript::Object* instance) override;

	virtual JingleScript::Object* GetReadInstance(JingleScript::Object* instance) override;
	virtual JingleScript::Object* GetWriteInstance(JingleScript::Object* instance) override;

};
