#include "Property/PropertyProperty.h"

#include "Property/PropertyArray.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(PropertyProperty)
	LINK_CONSTRUCTOR(Type*, Property*, uint64_t);
END_CLASS_LINK()

PropertyProperty::PropertyProperty(Type* type, Property* property, uint64_t offset)
	: PropertyBase(type, property), m_Offset(offset), m_PropertyData(nullptr)
{
	JS_TRACE(Tracers::Property);

}

PropertyProperty::~PropertyProperty()
{
	JS_TRACE(Tracers::Property);

}

bool PropertyProperty::OnDeserialize(Config* cfg)
{
	JS_TRACE(Tracers::Property);

	//! TODO: Figure out why the constructor isn't being called
	//m_PropertyData = m_PropertyType->New<PropertyBase>(m_PropertyType, m_Property, m_Offset);

	m_PropertyData = dynamic_cast<PropertyBase*>(NewObject<PropertyArray>("PropertyArray", m_PropertyType, m_Property, m_Offset));

	return m_PropertyData->OnDeserialize(cfg);
}

bool PropertyProperty::OnSerialize(Config* cfg)
{
	JS_TRACE(Tracers::Property);

	return m_PropertyData->OnSerialize(cfg);
}

bool PropertyProperty::OnReadObject(Object* instance)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Instance: {}", PointerToString(instance));

	return true;
}

bool PropertyProperty::OnWriteObject(Object* instance)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Instance: {}", PointerToString(instance));

	*(PropertyBase**)((char*)instance + m_Offset) = m_PropertyData;
	return true;
}

Object* PropertyProperty::GetReadInstance(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return instance;
}

Object* PropertyProperty::GetWriteInstance(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return instance;
}
