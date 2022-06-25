#include "Property/PropertyConfig.h"

using namespace JingleScript;

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

	return true;
}

#include "Scene/Scene.h"

bool PropertyConfig::OnWriteObject(Object* instance)
{
	JS_TRACE(Tracers::Property);

	Scene* scene = static_cast<Scene*>(instance);
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
