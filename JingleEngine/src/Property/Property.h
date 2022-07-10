#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

class PropertyArray;
class PropertyBase;

class Property : public JingleScript::Attribute
{
	DEFINE_CLASS(Property, JingleScript::Attribute);

private:
	bool m_OwnSerialization;
	bool m_IsDefaultNull;
	std::string m_DefaultValue;

public:
	JingleScript::Function<void, Config*> OnSerialize;
	JingleScript::Function<void, Config*> OnDeserialize;
	JingleScript::Function<void> OnRender;

public:
	Property()
	{ 
		JS_TRACE(Tracers::Property);

		m_OwnSerialization = false;
	}
	
	Property(bool useOwnSerialization)
	{
		JS_TRACE(Tracers::Property);

		m_OwnSerialization = useOwnSerialization;
	}
	
	Property(std::string defaultValue)
	{
		JS_TRACE(Tracers::Property);

		m_OwnSerialization = false;
		m_DefaultValue = defaultValue;
		m_IsDefaultNull = !m_DefaultValue.empty();
	}

	bool IsUsingOwnSerialization() const
	{
		return m_OwnSerialization;
	}

	bool IsDefaultNull() const
	{
		return m_IsDefaultNull;
	}
	
	bool IsValid() const
	{
		return !m_OwnSerialization || (OnSerialize.IsValid() && OnDeserialize.IsValid() && OnRender.IsValid());
	}

};

class ClassProperty : public JingleScript::Attribute
{
	DEFINE_CLASS(ClassProperty, JingleScript::Attribute);

public:	
	ClassProperty()
	{ 
		JS_TRACE(Tracers::Property);
	}
};
