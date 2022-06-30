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

	bool IsUsingOwnSerialization() const
	{
		return m_OwnSerialization;
	}
	
	bool IsValid() const
	{
		return !m_OwnSerialization || (OnSerialize.IsValid() && OnDeserialize.IsValid() && OnRender.IsValid());
	}

};
