#include "Property/PropertyBase.h"

PropertyBase::PropertyBase(JingleScript::Type* type, Property* property)
	: m_Type(type), m_Property(property)
{
	JS_TRACE(Tracers::Property);

}

PropertyBase::~PropertyBase()
{
	JS_TRACE(Tracers::Property);

}
