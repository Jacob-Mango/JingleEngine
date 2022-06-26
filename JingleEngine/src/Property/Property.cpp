#include "Property/Property.h"

#include "Config/ConfigArray.h"

#include "Property/PropertyArray.h"
#include "Property/PropertyAsset.h"
#include "Property/PropertyConfig.h"
#include "Property/PropertyItem.h"
#include "Property/PropertyObject.h"
#include "Property/PropertyProperty.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(Property);
	LINK_CONSTRUCTOR();
	LINK_CONSTRUCTOR(std::string);
END_CLASS_LINK();

Property::Property() : JingleScript::Attribute()
{
	m_TypeOverride = nullptr;
}

Property::Property(std::string type) : JingleScript::Attribute()
{
	m_TypeOverride = TypeManager::Get(type);
}

JingleScript::Type* Property::GetPropertyType()
{
	return m_TypeOverride == nullptr ? GetOwner() : m_TypeOverride;
}

PropertyBase* Property::CreateContainer(std::string cfgTypeName, uint64_t offset)
{
	JS_TRACE(Tracers::Property);

	Type* type = GetPropertyType();

	if (type->IsInherited(ConfigArray::StaticType()))
	{
		return NewObject<PropertyConfig>("PropertyConfig", type, this, offset);
	}
	
	if (!cfgTypeName.empty())
	{
		Type* overrideType = TypeManager::Get(cfgTypeName);
		if (overrideType == nullptr)
		{
			JS_TINFO("Override type doesn't exist.");
			return false;
		}

		if (!overrideType->IsInherited(type))
		{
			JS_TINFO("Override type '{}' doesn't inherit from property type '{}'.", overrideType->Name(), type->Name());
			//return false;
		}

		type = overrideType;
	}

	return CreateContainer(type, offset);
}

PropertyBase* Property::CreateContainer(Type* type, uint64_t offset)
{
	JS_TRACE(Tracers::Property);

	if (type->IsInherited(Array::StaticType()))
	{
		return NewObject<PropertyArray>("PropertyArray", type, this, offset);
	}
	else if (type->IsInherited(Asset::StaticType()))
	{
		return NewObject<PropertyAsset>("PropertyAsset", type, this, offset);
	}
	else if (type->IsStructure())
	{
		return NewObject<PropertyItem>("PropertyItem", type, this, offset);
	}

	return NewObject<PropertyObject>("PropertyObject", type, this, offset);
}

BEGIN_CLASS_LINK(ArrayProperty);
	LINK_CONSTRUCTOR(JingleScript::String);
END_CLASS_LINK();

ArrayProperty::ArrayProperty(JingleScript::String templateType)
{
	JS_TRACE(Tracers::Property);

	m_TemplateType = JingleScript::TypeManager::Get(templateType);
}
