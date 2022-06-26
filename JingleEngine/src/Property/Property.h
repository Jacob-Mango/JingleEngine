#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

class PropertyArray;
class PropertyBase;

class Property : public JingleScript::Attribute
{
	DEFINE_CLASS(Property, JingleScript::Attribute);

protected:
	JingleScript::Type* m_TypeOverride;

public:
	Property();
	Property(std::string type);

	JingleScript::Type* GetPropertyType();

	PropertyBase* CreateContainerDefault(std::string cfgTypeName, uint64_t offset);

	virtual PropertyBase* CreateContainer(std::string cfgTypeName, uint64_t offset);
	virtual PropertyBase* CreateContainer(JingleScript::Type* type, uint64_t offset);
};

class ArrayProperty : public Property
{
	DEFINE_CLASS(ArrayProperty, Property);

	friend PropertyArray;

public:
	ArrayProperty(std::string templateType);

	virtual PropertyBase* CreateContainer(std::string cfgTypeName, uint64_t offset) override;

};
