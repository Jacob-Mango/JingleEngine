#include "Property/StructureProperty.h"

#include "Config/Config.h"
#include "Config/ConfigArray.h"
#include "Config/ConfigAsset.h"
#include "Config/ConfigSection.h"
#include "Config/ConfigValue.h"

using namespace JingleScript;

bool StructureProperty::OnSerialize(Config* cfgRoot, void*& data)
{
	JS_TRACE(Tracers::Property);

	std::string value = ToString(data);

	//! TODO: compare with default and return 'false' if it is default

	cfgRoot->CreateValue(GetPropertyAttribute()->GetName(), value);

	return true;
}

bool StructureProperty::OnDeserialize(Config* cfg, void*& data)
{
	JS_TRACE(Tracers::Property);

	std::string& value = *cfg->GetValuePtr();
	return FromString(value, data);
}

std::string StructureProperty::ToString(void*& data)
{
	uint64_t size = GetPropertyType()->GetReferenceSize();

	FunctionSignature signature;
	signature.Name = "ToString";
	signature.Owner = GetPropertyType();
	signature.ReturnType = String::StaticType();

	auto function = signature.Find();
	if (!function)
	{
		JS_ERROR("Failed to find '{}'", signature.ToString());
		return false;
	}

	Thread* thread = Thread::Current();
	ValueStack* stack = &thread->Stack;
	
	uint64_t returnSize = String::StaticType()->GetReferenceSize();
	uint64_t offset = returnSize > size ? returnSize : size;

	stack->Push(offset);

	ValueData dst;
	dst.Size = size;
	dst.Offset = offset;
	dst.IsPointer = !GetPropertyType()->IsStructure();

	stack->CopyFrom(dst, data);

	function->Call(thread);

	std::string& value = *(std::string*)thread->Stack.Get(offset);

	//! TODO: call string destructor to free the memory

	stack->Pop(offset);

	return value;
}

bool StructureProperty::FromString(std::string& value, void*& data)
{
	uint64_t size = GetPropertyType()->GetReferenceSize();

	if (GetPropertyType() == String::StaticType())
	{
		memcpy(data, &value, size);
		return true;
	}

	FunctionSignature signature;
	signature.Name = "FromString";
	signature.Owner = nullptr;
	signature.ReturnType = GetPropertyType();
	signature.Parameters[0] = { String::StaticType() };

	auto function = signature.Find();
	if (!function)
	{
		JS_ERROR("Failed to find '{}'", signature.ToString());
		return false;
	}

	Thread* thread = Thread::Current();
	ValueStack* stack = &thread->Stack;

	stack->Push(size);

	uint64_t offset = size;
	Function_Parameter(stack, value, offset);

	function->Call(thread);

	memcpy(data, stack->Get(offset), size);
	stack->Pop(offset);
	return true;
}

void StructureProperty::Editor_OnRender(void*& data)
{
	std::string name = GetPropertyAttribute()->GetName();
	Editor::Render_CellHeader(name);

	std::string value = ToString(data);;
	std::string previous = value;
	if (Editor::Render_CellInputText(value))
	{
		JS_INFO("Value changed from {} to {}", previous, value);

		//JingleScript::Function<void, std::string> Script_Editor_OnPropertyChanged = { "Editor_OnPropertyChanged", GetPropertyOwner()->GetPropertyType() };
		//Script_Editor_OnPropertyChanged[GetPropertyOwner()](name);
		GetPropertyOwner()->Editor_OnPropertyChanged(name);
	}
}
