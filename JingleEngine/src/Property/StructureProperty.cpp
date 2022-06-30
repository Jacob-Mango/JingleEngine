#include "Property/StructureProperty.h"

using namespace JingleScript;

bool StructureProperty::OnSerialize(Config* cfg, void*& data)
{
	JS_TRACE(Tracers::Property);

	uint64_t size = GetPropertyType()->GetReferenceSize();

	FunctionSignature signature;
	signature.Name = "ToString";
	signature.Owner = GetPropertyType();
	signature.ReturnType = String::StaticType();

	auto function = signature.Find();
	if (!function)
	{
		std::cerr << "Failed to find 'ToString'" << std::endl;
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

	std::string* value = (std::string*)thread->Stack.Get(offset);
	cfg->SetValue(*value);

	stack->Pop(offset);

	return true;
}

bool StructureProperty::OnDeserialize(Config* cfg, void*& data)
{
	JS_TRACE(Tracers::Property);

	uint64_t size = GetPropertyType()->GetReferenceSize();

	std::string& value = *cfg->GetValuePtr();

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
		std::cerr << "Failed to find 'FromString'" << std::endl;
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
