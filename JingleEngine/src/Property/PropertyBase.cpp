#include "Property/PropertyBase.h"

using namespace JingleScript;

BEGIN_VIRTUAL_CLASS_LINK(PropertyBase)
END_VIRTUAL_CLASS_LINK()

PropertyBase::PropertyBase(JingleScript::Type* type, Property* property)
	: m_PropertyType(type), m_Property(property)
{
	JS_TRACE(Tracers::Property);

}

PropertyBase::~PropertyBase()
{
	JS_TRACE(Tracers::Property);

}

bool PropertyBase::Deserialize(Config* cfg, PropertyBase* parent)
{
	m_Config = cfg;
	m_Parent = parent;

	return OnDeserialize(m_Config);
}

bool PropertyBase::Serialize(Config* cfg)
{
	return OnSerialize(cfg);
}

Type* PropertyBase::GetPropertyType() const
{
	return m_PropertyType;
}

Property* PropertyBase::GetPropertyAttribute() const
{
	return m_Property;
}
