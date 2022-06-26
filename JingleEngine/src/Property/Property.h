#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

class PropertyArray;
class PropertyBase;

class Property : public JingleScript::Attribute
{
	DEFINE_CLASS(Property, JingleScript::Attribute);

	JingleScript::Type* m_TypeOverride;

public:
	Property();
	Property(std::string type);

	JingleScript::Type* GetPropertyType();

	PropertyBase* CreateContainer(std::string cfgTypeName, uint64_t offset);
	PropertyBase* CreateContainer(JingleScript::Type* type, uint64_t offset);
};

class ArrayProperty : public Property
{
	DEFINE_CLASS(ArrayProperty, Property);

	friend PropertyArray;

private:
	JingleScript::Type* m_TemplateType;

public:
	ArrayProperty(JingleScript::String templateType);

	virtual std::string ToString() const { return m_TemplateType->Name(); }

};
