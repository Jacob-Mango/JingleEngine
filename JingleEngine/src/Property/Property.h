#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

class Property : public JingleScript::Attribute
{
	DEFINE_CLASS(Property, JingleScript::Attribute);

public:
	Property() : JingleScript::Attribute() {}

};

class ArrayProperty : public Property
{
	DEFINE_CLASS(ArrayProperty, Property);

	friend class PropertyArray;

	std::string m_TemplateType;

public:
	ArrayProperty() {}
	ArrayProperty(JingleScript::String templateType) : m_TemplateType(templateType) {}

	virtual std::string ToString() const { return m_TemplateType; }

};
