#include "Property/ObjectProperty.h"

#include "Config/Config.h"
#include "Config/ConfigAsset.h"
#include "Config/ConfigSection.h"
#include "Config/ConfigValue.h"

#include "Property/AssetProperty.h"
#include "Property/ArrayProperty.h"
#include "Property/BaseProperty.h"
#include "Property/StructureProperty.h"

using namespace JingleScript;

template<typename T>
T* GetAttribute(Type::VariableDefinition* variable)
{
	JS_TRACE(Tracers::Property);

	for (auto& attributeBase : variable->Attributes)
	{
		if (attributeBase->GetType()->IsInherited(T::StaticType()))
		{
			return static_cast<T*>(attributeBase);
		}
	}

	return nullptr;
}

template<typename T>
T* GetAttribute(Type* type)
{
	JS_TRACE(Tracers::Property);

	for (auto& attributeBase : type->Attributes)
	{
		if (attributeBase->GetType()->IsInherited(T::StaticType()))
		{
			return static_cast<T*>(attributeBase);
		}
	}

	return nullptr;
}

bool ObjectProperty::OnSerialize(Config* cfgRoot, void*& data)
{
	JS_TRACE(Tracers::Property);
	
	Object* object = static_cast<Object*>(data);
	if (object == nullptr)
	{
		return true;
	}

	Type* type = object->GetType();

	Config* cfg = cfgRoot;
	if (GetPropertyAttribute())
	{
		cfg = cfgRoot->CreateSection(GetName());
	}
	else
	{
		cfg->SetName(GetName());
	}

	if (type != GetPropertyType())
	{
		cfg->SetLinkedType(type->Name());
	}
	else if (GetPropertyType())
	{
		cfg->SetLinkedType(GetPropertyType()->Name());
	}
	else
	{
		cfg->SetLinkedType("");
	}

	cfg->SetBaseAsset(GetBaseConfig());

	for (auto& [varName, property] : m_Properties)
	{
		auto varType = property->GetPropertyType();
		auto varOffset = property->GetPropertyOffset();
		auto varProperty = property->GetPropertyAttribute();

		void*& data = *(void**)((char*)object + property->m_Offset);
		if (varType->IsStructure())
		{
			data = (void*)((char*)object + property->m_Offset);
		}

		if (varProperty->IsUsingOwnSerialization())
		{
			varProperty->OnSerialize[this](cfg);
		}
		else
		{
			property->OnSerialize(cfg, data);
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

	if (cfg)
	{
		SetName(cfg->GetName());
	}
	else
	{
		SetName(GetPropertyAttribute()->GetName());
	}

	SetBaseConfig(cfg->GetBaseAsset());

	for (auto& variable : type->GetVariables())
	{
		auto varName = variable->Name;
		auto varType = variable->Type;
		auto varOffset = variable->Offset;
		auto varProperty = GetAttribute<Property>(variable);

		JS_TINFO("Variable {}:{}", varType->Name(), varName);

		if (!varProperty)
		{
			JS_TINFO("Skipping, not a property.");
			continue;
		}

		if (varProperty->IsUsingOwnSerialization())
		{
			varProperty->OnSerialize = { "OnSerialize" + varName, type };
			varProperty->OnDeserialize = { "OnDeserialize" + varName, type };
			varProperty->OnRender = { "Editor_OnRender" + varName, type };
		}

		if (!varProperty->IsValid())
		{
			JS_TINFO("Skipping, property not valid.");
			continue;
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

		if (varProperty->IsUsingOwnSerialization())
		{
			varProperty->OnDeserialize[this](cfg);
		}
		else
		{
			void*& data = *(void**)((char*)object + varOffset);
			if (varType->IsStructure())
			{
				data = (void*)((char*)object + varOffset);
			}
			
			property->OnDeserialize(cfgVariable, data);
		}

		m_Properties.insert({ varName, property });
	}

	return true;
}

void ObjectProperty::Editor_OnRender(void*& data)
{
	Object* object = static_cast<Object*>(data);
	if (object == nullptr)
	{
		return;
	}

	Type* type = object->GetType();

	if (GetPropertyAttribute())
	{
		Editor::Render_CellHeader(GetPropertyAttribute()->GetName());

		ImGui::TableNextColumn();
		ImGui::TextUnformatted(type->Name().c_str());
	}

	for (auto& variable : type->GetVariables())
	{
		auto varName = variable->Name;

		auto it = m_Properties.find(varName);
		if (it == m_Properties.end())
		{
			continue;
		}

		auto property = it->second;

		ScopedIncrement increment(Editor::Context.Depth);

		auto varType = property->GetPropertyType();
		auto varOffset = property->GetPropertyOffset();
		auto varProperty = property->GetPropertyAttribute();

		void*& data = *(void**)((char*)object + varOffset);
		std::string id = PointerToString(data);

		if (varType->IsStructure())
		{
			data = (void*)((char*)object + varOffset);
		}

		if (!varProperty->IsValid())
		{
			continue;
		}

		ImGui::PushID(id.c_str());

		if (varProperty->IsUsingOwnSerialization())
		{
			varProperty->OnRender[object]();
		}
		else
		{
			property->Editor_OnRender(data);
		}

		ImGui::PopID();
	}
}

ConfigAsset* ObjectProperty::GetBaseConfig() const
{
	return m_BaseConfig;
}

void ObjectProperty::SetBaseConfig(ConfigAsset* asset)
{
	m_BaseConfig = asset;
}

const std::string& ObjectProperty::GetName() const
{
	return m_Name;
}

void ObjectProperty::SetName(const std::string& name)
{
	m_Name = name;
}

Config* ObjectProperty::Serialize()
{
	Config* cfg = NewObject<ConfigSection>("ConfigSection")->As<Config>();
	void* object = (void*)dynamic_cast<Object*>(this);
	if (!OnSerialize(cfg, object))
	{
		return nullptr;
	}

	return cfg;
}

bool ObjectProperty::Deserialize(Config* cfg)
{
	void* object = (void*)dynamic_cast<Object*>(this);
	return OnDeserialize(cfg, object);
}

void ObjectProperty::Editor_Render()
{
	void* object = (void*)dynamic_cast<Object*>(this);
	Editor_OnRender(object);
}
