#include "Property/ObjectProperty.h"

#include "Property/AssetProperty.h"
#include "Property/ArrayProperty.h"
#include "Property/BaseProperty.h"
#include "Property/StructureProperty.h"

using namespace JingleScript;

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

bool ObjectProperty::OnSerialize(Config* cfg, void*& data)
{
	JS_TRACE(Tracers::Property);
	
	Object* object = static_cast<Object*>(data);
	if (object == nullptr)
	{
		//! TODO: Remove entry
		return true;
	}

	Type* type = object->GetType();

	cfg->SetLinkedType(type->Name());

	for (auto& [varName, propertyPair] : m_Properties)
	{
		auto& [property, pointer] = propertyPair;
		auto varType = property->GetPropertyType();
		auto varOffset = property->GetPropertyOffset();
		auto varProperty = property->GetPropertyAttribute();

		Config* cfgVariable = cfg->Get(varName);
		if (!cfgVariable)
		{
			//! TODO: create the config entry and compare to see if it should exist and be added or be removed.

			continue;
		}

		void* data = (void*)((char*)object + property->m_Offset);
		if (pointer)
		{
			data = *(void**)((char*)object + property->m_Offset);
		}
		
		if (varProperty->OnSerialize.IsValid())
		{
			varProperty->OnSerialize[this](cfgVariable);
		}
		else
		{
			property->OnSerialize(cfgVariable, data);
		}
	}

	return true;
}

bool ObjectProperty::OnDeserialize(Config* cfg, void*& data)
{
	JS_TRACE(Tracers::Property);

	m_Properties.clear();

	Object* object = static_cast<Object*>(data);
	if (object == nullptr)
	{
		JS_ERROR("Data is null");
		return false;
	}

	Type* type = object->GetType();

	for (auto& variable : type->GetVariables())
	{
		auto varName = variable->Name;
		auto varType = variable->Type;
		auto varOffset = variable->Offset;
		auto varProperty = FindProperty(variable);

		JS_TINFO("Variable {}:{}", varType->Name(), varName);

		if (!varProperty)
		{
			JS_TINFO("Skipping, not a property.");
			continue;
		}

		if (!varProperty->m_OnSerialize.empty())
		{
			varProperty->OnSerialize = { varProperty->m_OnSerialize, type };
		}

		if (!varProperty->m_OnDeserialize.empty())
		{
			varProperty->OnDeserialize = { varProperty->m_OnDeserialize, type };
		}

		Config* cfgVariable = cfg ? cfg->Get(varName) : nullptr;

		BaseProperty* property = nullptr;

		if (varType->IsInherited(Asset::StaticType()))
		{
			JS_TINFO("Asset");
			property = new AssetProperty(this, varType, varProperty);
		}
		else if (varType->IsStructure())
		{
			JS_TINFO("Structure");
			property = new StructureProperty(this, varType, varProperty);
		}

		if (!property)
		{
			bool isArray = false;

			if (varType->IsInherited(ArrayBase::StaticType()))
			{
				JS_TINFO("Array");

				property = new ArrayProperty(this, varType, varProperty);
				isArray = true;
			}
			else
			{
				JS_TINFO("Object");

				property = new ObjectProperty(this, varType, varProperty);
			}

			if (cfgVariable || isArray)
			{
				Type* type = varType;
				std::string cfgVarType = cfgVariable->GetLinkedType();
				if (!cfgVarType.empty())
				{
					type = TypeManager::Get(cfgVarType);
				}
				
				*(Object**)((char*)object + varOffset) = type->New<Object>();
			}
			else
			{
				*(Object**)((char*)object + varOffset) = nullptr;
			}
		}

		property->m_Offset = varOffset;

		bool isPropertyPointer = !varType->IsStructure();

		if (varProperty->OnDeserialize.IsValid())
		{
			varProperty->OnDeserialize[this](cfgVariable);
		}
		else
		{
			void*& data = *(void**)((char*)object + varOffset);
			if (!isPropertyPointer)
			{
				data = (void*)((char*)object + varOffset);
			}
			
			property->OnDeserialize(cfgVariable, data);
		}

		m_Properties.insert({ varName, { property, isPropertyPointer } });
	}

	return true;
}

bool ObjectProperty::Serialize(Config* cfg)
{
	void* object = (void*)dynamic_cast<Object*>(this);
	return OnSerialize(cfg, object);
}

bool ObjectProperty::Deserialize(Config* cfg)
{
	void* object = (void*)dynamic_cast<Object*>(this);
	return OnDeserialize(cfg, object);
}
