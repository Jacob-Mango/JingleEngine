#include "Property/PropertyObject.h"

#include "Property/PropertyAsset.h"
#include "Property/PropertyArray.h"
#include "Property/PropertyItem.h"
#include "Property/PropertyProperty.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(PropertyObject)
	LINK_CONSTRUCTOR();
	LINK_CONSTRUCTOR(Type*, Property*, uint64_t);
END_CLASS_LINK()

PropertyObject::PropertyObject()
	: PropertyObject(nullptr, nullptr, 0)
{
	JS_TRACE(Tracers::Property);

}

PropertyObject::PropertyObject(Type* type, Property* property, uint64_t offset)
	: PropertyBase(type, property), m_Offset(offset)
{
	JS_TRACE(Tracers::Property);

}

PropertyObject::~PropertyObject()
{
	JS_TRACE(Tracers::Property);

}

Property* FindProperty(Type::VariableDefinition* variable)
{
	JS_TRACE(Tracers::Property);

	for (auto& attributeBase : variable->Attributes)
	{
		if (attributeBase->GetType()->IsInherited(Property::StaticType()))
		{
			return static_cast<Property*>(attributeBase);
		}
	}

	return nullptr;
}

bool PropertyObject::OnDeserialize(Config* cfg)
{
	JS_TRACE(Tracers::Property);

	m_Properties.clear();

	Type* newType = TypeManager::Get(cfg->GetLinkedType());
	if (newType == nullptr)
	{
		if (m_PropertyType == nullptr)
		{
			JS_TINFO("Both config type and property type not valid.");
			return false;
		}
	}
	else if (m_PropertyType == nullptr)
	{
		m_PropertyType = newType;
	}
	else if (!newType->IsInherited(m_PropertyType))
	{
		//return false;
	}

    auto variables = m_PropertyType->GetVariables();

    for (auto& variable : variables)
    {
		auto& name = variable->Name;
		auto& type = variable->Type;
		auto& offset = variable->Offset;
		Property* property = FindProperty(variable);

		if (!property)
		{
			continue;
		}

		Config* cfgVariable = cfg->Get(name);
		if (!cfgVariable)
		{
			continue;
		}

		std::string typeStr = cfgVariable->GetLinkedType();

		PropertyBase* propertyData = nullptr;
		if (property->GetOwner()->IsInherited(PropertyBase::StaticType()))
		{
			propertyData = NewObject<PropertyProperty>("PropertyProperty", property->GetPropertyType(), property, (uint64_t)offset);
		}

		if (!propertyData)
		{
			propertyData = property->CreateContainer(typeStr, offset);
		}

		if (!propertyData)
		{
			JS_TINFO("Couldn't create container for type '{}'.", typeStr);
			return false;
		}

		if (!propertyData->OnDeserialize(cfgVariable))
		{
			return false;
		}

		m_Properties.insert({name, propertyData});
    }

    return true;
}

bool PropertyObject::OnSerialize(Config* cfg)
{
	JS_TRACE(Tracers::Property);

    auto variables = m_PropertyType->GetVariables();

    for (auto& variable : variables)
    {
		auto& name = variable->Name;
		auto& baseType = variable->Type;
		auto& offset = variable->Offset;

		auto it = m_Properties.find(name);
		if (it == m_Properties.end())
		{
			//! TODO: Add to property list
			continue;
		}

		Config* cfgVariable = cfg->Get(name);
		if (!cfgVariable)
		{
			continue;
		}

		it->second->OnSerialize(cfgVariable);
	}

    return true;
}

bool PropertyObject::OnReadObject(Object* instance)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Instance: {}", PointerToString(instance));

	for (auto& [name, property] : m_Properties)
	{
		Object* read = property->GetWriteInstance(instance);

		if (!property->OnReadObject(read))
		{
			return false;
		}
	}

	return true;
}

bool PropertyObject::OnWriteObject(Object* instance)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Instance: {}", PointerToString(instance));

	for (auto& [name, property] : m_Properties)
	{
		Object* write = property->GetWriteInstance(instance);
		
		if (!property->OnWriteObject(write))
		{
			return false;
		}
	}

	return true;
}

Object* PropertyObject::GetReadInstance(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return *(Object**)((char*)instance + m_Offset);
}

Object* PropertyObject::GetWriteInstance(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return m_PropertyType->New<Object>();
}
