#include "Property/PropertyObject.h"

#include "Property/PropertyArray.h"
#include "Property/PropertyItem.h"

using namespace JingleScript;

PropertyObject::PropertyObject()
	: PropertyObject(nullptr, nullptr, 0)
{

}

PropertyObject::PropertyObject(Type* type, Property* property, uint64_t offset)
	: PropertyBase(type, property), m_Offset(offset)
{

}

PropertyObject::~PropertyObject()
{

}

Property* FindProperty(Type::VariableDefinition* variable)
{
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
	m_Properties.clear();

	Type* newType = TypeManager::Get(cfg->GetType());
	if (newType == nullptr)
	{
		if (m_Type == nullptr)
		{
			return false;
		}
	}
	else if (m_Type == nullptr)
	{
		m_Type = newType;
	}
	else if (!newType->IsInherited(m_Type))
	{
		return false;
	}

    auto variables = m_Type->GetVariables();

    for (auto& variable : variables)
    {
		auto& name = variable->Name;
		auto& baseType = variable->Type;
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

		Type* type = baseType;
		std::string typeStr = cfgVariable->GetType();
		if (!typeStr.empty())
		{
			type = TypeManager::Get(typeStr);
			if (type == nullptr)
			{
				return false;
			}

			if (!type->IsInherited(baseType))
			{
				return false;
			}
		}

		if (type->IsInherited(Array::StaticType()))
		{
			PropertyArray* array = new PropertyArray(type, property, offset);

			if (!array->OnDeserialize(cfgVariable))
			{
				return false;
			}

			m_Properties.insert({name, array});
		}
		else if (type->IsStructure())
		{
			PropertyItem* item = new PropertyItem(type, property, offset);

			if (!item->OnDeserialize(cfgVariable))
			{
				return false;
			}

			m_Properties.insert({name, item});
		}
		else
		{
			PropertyObject* object = new PropertyObject(type, property, offset);

			if (!object->OnDeserialize(cfgVariable))
			{
				return false;
			}

			m_Properties.insert({name, object});
		}
    }

    return true;
}

bool PropertyObject::OnSerialize(Config* cfg)
{
    auto variables = m_Type->GetVariables();

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
	return *(Object**)((char*)instance + m_Offset);
}

Object* PropertyObject::GetWriteInstance(Object* instance)
{
	return m_Type->New<Object>();
}
