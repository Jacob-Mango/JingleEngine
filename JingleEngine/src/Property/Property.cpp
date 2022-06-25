#include "Property/Property.h"

#include "Config/ConfigArray.h"

#include "Property/PropertyArray.h"
#include "Property/PropertyAsset.h"
#include "Property/PropertyConfig.h"
#include "Property/PropertyItem.h"
#include "Property/PropertyObject.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(Property);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

PropertyBase* Property::CreateContainer(std::string cfgTypeName, uint64_t offset)
{
	JS_TRACE(Tracers::Property);

	PropertyBase* container = nullptr;

	Type* type = GetOwner();

	if (type->IsInherited(ConfigArray::StaticType()))
	{
		return new PropertyConfig(type, this, offset);
	}

	if (!cfgTypeName.empty())
	{
		Type* overrideType = TypeManager::Get(cfgTypeName);
		if (overrideType == nullptr)
		{
			return nullptr;
		}

		if (!overrideType->IsInherited(type))
		{
			return nullptr;
		}

		type = overrideType;
	}

	if (type->IsInherited(Array::StaticType()))
	{
		return new PropertyArray(type, this, offset);
	}
	else if (type->IsInherited(Asset::StaticType()))
	{
		return new PropertyAsset(type, this, offset);
	}
	else if (type->IsStructure())
	{
		return new PropertyItem(type, this, offset);
	}

	return new PropertyObject(type, this, offset);
}

BEGIN_CLASS_LINK(ArrayProperty);
	LINK_CONSTRUCTOR(JingleScript::String);
END_CLASS_LINK();

ArrayProperty::ArrayProperty(JingleScript::String templateType)
{
	JS_TRACE(Tracers::Property);

	m_TemplateType = JingleScript::TypeManager::Get(templateType);
}
