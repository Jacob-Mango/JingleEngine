#include "Property/PropertyObject.h"

#include "Property/PropertyItem.h"

using namespace JingleScript;

PropertyObject::PropertyObject()
	: PropertyObject(nullptr, nullptr, 0)
{

}

PropertyObject::PropertyObject(JingleScript::Type* type, Property* property, uint64_t offset)
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
		if (attributeBase->GetType() == Property::StaticType())
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
	if (m_Type == nullptr || newType->IsInherited(m_Type))
	{
		m_Type = newType;
	}
	else
	{
		return false;
	}

    auto variables = m_Type->GetVariables();

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

		if (type->IsStructure())
		{
			PropertyItem* item = new PropertyItem(type, property, offset);

			if (!item->OnDeserialize(cfgVariable))
			{
				return false;
			}

			m_Properties.push_back(item);
		}
		else
		{
			//! TODO: 
		}
    }

    return true;
}

bool PropertyObject::OnSerialize(Config* cfg)
{
    return true;
}

void PropertyObject::OnReadObject(JingleScript::Object* instance)
{
	ReadObject(instance);
}

void PropertyObject::OnWriteObject(JingleScript::Object* instance)
{
	WriteObject(instance);
}

void PropertyObject::ReadObject(JingleScript::Object* instance)
{
	for (auto& property : m_Properties)
	{
		property->OnReadObject(instance);
	}
}

void PropertyObject::WriteObject(JingleScript::Object* instance)
{
	for (auto& property : m_Properties)
	{
		property->OnWriteObject(instance);
	}
}
