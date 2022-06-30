#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

class PropertyArray;
class PropertyBase;

class Property : public JingleScript::Attribute
{
	DEFINE_CLASS(Property, JingleScript::Attribute);

public:
	JingleScript::Function<void, Config*> OnSerialize;
	JingleScript::Function<void, Config*> OnDeserialize;

public:
	Property()
	{ 
		JS_TRACE(Tracers::Property);
	}
	
	Property(std::string onSerialize, std::string onDeserialize)
	{
		JS_TRACE(Tracers::Property);
		
		OnSerialize = { onSerialize, GetType() };
		OnDeserialize = { onDeserialize, GetType() };
	}

};
