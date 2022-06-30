#include "Property/BaseProperty.h"

BaseProperty::BaseProperty()
{
	JS_TRACE(Tracers::Property);

	m_Owner = nullptr;
	m_propType = nullptr;
	m_VariableProperty = nullptr;
}

BaseProperty::BaseProperty(BaseProperty* parent, JingleScript::Type* type, Property* varProperty)
{
	JS_TRACE(Tracers::Property);

	m_Owner = parent;
	m_propType = type;
	m_VariableProperty = varProperty;
}

BaseProperty::~BaseProperty()
{
	JS_TRACE(Tracers::Property);

}

BaseProperty* BaseProperty::GetPropertyOwner() const
{
	JS_TRACE(Tracers::Property);

	return m_Owner;
}

JingleScript::Type* BaseProperty::GetPropertyType() const
{
	JS_TRACE(Tracers::Property);
	
	return m_propType;
}

Property* BaseProperty::GetPropertyAttribute() const
{
	JS_TRACE(Tracers::Property);
	
	return m_VariableProperty;
}

int64_t BaseProperty::GetPropertyOffset() const
{
	JS_TRACE(Tracers::Property);
	
	return m_Offset;
}
