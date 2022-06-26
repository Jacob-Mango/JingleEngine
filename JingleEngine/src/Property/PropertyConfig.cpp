#include "Property/PropertyConfig.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(PropertyConfig)
	LINK_CONSTRUCTOR(Type*, Property*, uint64_t);
END_CLASS_LINK()

PropertyConfig::PropertyConfig(Type* type, Property* property, uint64_t offset)
	: PropertyBase(type, property), m_Offset(offset), m_Config(nullptr)
{
	JS_TRACE(Tracers::Property);

}

PropertyConfig::~PropertyConfig()
{
	JS_TRACE(Tracers::Property);

}

bool PropertyConfig::OnDeserialize(Config* cfg)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Deserializing {}", cfg->GetTypeAndName());

	m_Config = cfg;

	return true;
}

bool PropertyConfig::OnSerialize(Config* cfg)
{
	JS_TRACE(Tracers::Property);

	//! TODO: compare the two pointers and throw an error if they no longer match

	return true;
}

bool PropertyConfig::OnReadObject(Object* instance)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Instance: {}", PointerToString(instance));

	return true;
}

bool PropertyConfig::OnWriteObject(Object* instance)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Instance: {}", PointerToString(instance));

	*(Config**)((char*)instance + m_Offset) = m_Config;
	return true;
}

Object* PropertyConfig::GetReadInstance(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return instance;
}

Object* PropertyConfig::GetWriteInstance(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return instance;
}
