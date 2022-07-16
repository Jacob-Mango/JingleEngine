#include "Property/ArrayProperty.h"

#include "Config/Config.h"
#include "Config/ConfigAsset.h"
#include "Config/ConfigSection.h"
#include "Config/ConfigValue.h"

#include "Property/AssetProperty.h"
#include "Property/BaseProperty.h"
#include "Property/StructureProperty.h"
#include "Property/ObjectProperty.h"

using namespace JingleScript;

bool ArrayProperty::OnSerialize(Config* cfgRoot, void*& data)
{
	JS_TRACE(Tracers::Property);

	Object* object = static_cast<Object*>(data);
	if (object == nullptr)
	{
		JS_ERROR("Data is null");
		return false;
	}

	Type* type = object->GetType();
	uint64_t typeSize = type->GetReferenceSize();

	Config* cfg = cfgRoot->CreateArray(GetPropertyAttribute()->GetName(), type != GetPropertyType() ? type : nullptr);

	Function<std::string> Script_GetDataType = { "GetDataType", type };
	Function<int> Script_Count = { "Count", type };

	if (!Script_Count.IsValid() || !Script_GetDataType.IsValid())
	{
		JS_ERROR("Array functions do not exist!");
		return false;
	}

	Type* varType = TypeManager::Get(Script_GetDataType[object]());
	uint64_t varSize = varType->GetReferenceSize();

	AbstractFunction* Script_Get = nullptr;
	{
		FunctionSignature getFunctionSignature;
		getFunctionSignature.Name = "Get";
		getFunctionSignature.Owner = type;
		getFunctionSignature.ReturnType = varType;
		getFunctionSignature[0] = { Integer::StaticType() };
		Script_Get = getFunctionSignature.Find();
	}

	if (!Script_Get)
	{
		JS_ERROR("Array functions do not exist!");
		return false;
	}

	cfg->SetLinkedType(type->Name());

	Thread* thread = Thread::Current();
	ValueStack* stack = &thread->Stack;

	uint64_t size = varSize > typeSize ? varSize : typeSize;
	uint64_t intSize = Integer::StaticType()->GetReferenceSize();

	stack->Push(size);
	stack->Push(intSize);

	int count = Script_Count[object]();
	for (int i = 0; i < count; i++)
	{
		stack->CopyFrom(size + intSize, &data, typeSize);
		stack->CopyFrom(intSize, &i, intSize);

		Script_Get->Call(thread);

		void* dta = *(void**)stack->Get(size + intSize);
		Object* obj = static_cast<Object*>(dta);

		ObjectProperty* property = dynamic_cast<ObjectProperty*>(obj);
		if (property)
		{
			Config* cfgItem = cfg->CreateSection(property->GetName());
			property->OnSerialize(cfgItem, dta);
			continue;
		}

		m_PropertyData->OnSerialize(cfg, dta);
	}

	stack->Pop(intSize);
	stack->Pop(size);

	return true;
}

bool ArrayProperty::OnDeserialize(Config* cfg, void*& data)
{
	JS_TRACE(Tracers::Property);

	if (!cfg)
	{
		return true;
	}

	Object* object = static_cast<Object*>(data);
	if (object == nullptr)
	{
		JS_ERROR("Data is null");
		return false;
	}

	Type* type = object->GetType();
	uint64_t typeSize = type->GetReferenceSize();

	// Get functions
	Function<void> Script_Clear = { "Clear", type };
	Function<std::string> Script_GetDataType = { "GetDataType", type };

	if (!Script_Clear.IsValid() || !Script_GetDataType.IsValid())
	{
		JS_ERROR("Array functions do not exist!");
		return false;
	}

	Type* varType = TypeManager::Get(Script_GetDataType[object]());
	uint64_t varSize = varType->GetReferenceSize();

	m_PropertyData = new ObjectProperty(this, varType, GetPropertyAttribute());

	AbstractFunction* Script_Insert = nullptr;
	{
		FunctionSignature insertFunctionSignature;
		insertFunctionSignature.Name = "Insert";
		insertFunctionSignature.Owner = type;
		insertFunctionSignature.ReturnType = nullptr;
		insertFunctionSignature[0] = { varType };
		Script_Insert = insertFunctionSignature.Find();
	}

	if (!Script_Insert)
	{
		JS_ERROR("Array functions do not exist!");
		return false;
	}

	//! The actual entries won't be cleared from the array, we will need to somehow delete those as well
	Script_Clear[object]();
	
	Thread* thread = Thread::Current();
	ValueStack* stack = &thread->Stack;

	stack->Push(typeSize);
	stack->CopyFrom(typeSize, &data, typeSize);

	for (auto& cfgVariable : *cfg)
	{
		Object* object = varType->New<Object>();

		//! TODO: structure and asset support
		void* data = (void*)object;

		ObjectProperty* property = dynamic_cast<ObjectProperty*>(object);
		if (property)
		{
			property->m_VariableProperty = m_PropertyData->GetPropertyAttribute();
			property->OnDeserialize(&cfgVariable, data);
		}
		else
		{
			m_PropertyData->OnDeserialize(&cfgVariable, data);
		}

		stack->Push(varSize);
		stack->CopyFrom(varSize, &data, varSize);
		Script_Insert->Call(thread);
		stack->Pop(varSize);
	}

	stack->Pop(typeSize);
	return true;
}

void ArrayProperty::Editor_OnRender(void*& data)
{
#ifdef JE_EDITOR
	Object* object = static_cast<Object*>(data);
	if (object == nullptr)
	{
		JS_ERROR("Data is null");
		return;
	}

	Type* type = object->GetType();
	uint64_t typeSize = type->GetReferenceSize();

	Function<std::string> Script_GetDataType = { "GetDataType", type };
	Function<int> Script_Count = { "Count", type };

	if (!Script_Count.IsValid() || !Script_GetDataType.IsValid())
	{
		JS_ERROR("Array functions do not exist!");
		return;
	}

	Type* varType = TypeManager::Get(Script_GetDataType[object]());
	uint64_t varSize = varType->GetReferenceSize();

	AbstractFunction* Script_Get = nullptr;
	{
		FunctionSignature getFunctionSignature;
		getFunctionSignature.Name = "Get";
		getFunctionSignature.Owner = type;
		getFunctionSignature.ReturnType = varType;
		getFunctionSignature[0] = { Integer::StaticType() };
		Script_Get = getFunctionSignature.Find();
	}

	if (!Script_Get)
	{
		JS_ERROR("Array functions do not exist!");
		return;
	}

	Thread* thread = Thread::Current();
	ValueStack* stack = &thread->Stack;

	stack->Push(typeSize);
	stack->CopyFrom(typeSize, &data, typeSize);

	int count = Script_Count[object]();
	for (int i = 0; i < count; i++)
	{
		ScopedIncrement increment(EditorUI::Context.Depth);

		stack->Push(sizeof(int));
		stack->CopyFrom(sizeof(int), &i, sizeof(int));
		Script_Get->Call(thread);
		stack->Pop(sizeof(int));

		void* data = stack->Get(varSize > typeSize ? varSize : typeSize);

		m_PropertyData->Editor_OnRender(data);
	}
#endif
}
