#include "Property/PropertyItem.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(PropertyItem)
	LINK_CONSTRUCTOR(Type*, Property*, uint64_t);
END_CLASS_LINK()

PropertyItem::PropertyItem(Type* type, Property* property, uint64_t offset)
	: PropertyBase(type, property), m_Offset(offset), m_Data(nullptr)
{
	JS_TRACE(Tracers::Property);

}

PropertyItem::~PropertyItem()
{
	JS_TRACE(Tracers::Property);

}

bool PropertyItem::OnDeserialize(Config* cfg)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Deserializing {}", cfg->GetTypeAndName());

	uint64_t size = m_PropertyType->GetReferenceSize();

	m_Data = malloc(size);
	if (!m_Data)
	{
		JS_TINFO("Couldn't allocate memory.");
		return false;
	}

	std::string& value = *cfg->GetValuePtr();

	if (m_PropertyType == String::StaticType())
	{
		memcpy(m_Data, &value, size);
		return true;
	}

	FunctionSignature signature;
	signature.Name = "FromString";
	signature.Owner = nullptr;
	signature.ReturnType = m_PropertyType;
	signature.Parameters.push_back({ String::StaticType() });

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

	memcpy(m_Data, stack->Get(offset), size);
	stack->Pop(offset);

	return true;
}

bool PropertyItem::OnSerialize(Config* cfg)
{
	JS_TRACE(Tracers::Property);

	uint64_t size = m_PropertyType->GetReferenceSize();

	FunctionSignature signature;
	signature.Name = "ToString";
	signature.Owner = m_PropertyType;
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
	dst.IsPointer = !m_PropertyType->IsStructure();

	stack->CopyFrom(dst, m_Data);

	function->Call(thread);

	std::string* value = (std::string*)thread->Stack.Get(offset);
	cfg->SetValue(*value);

	stack->Pop(offset);

	return true;
}

bool PropertyItem::OnReadObject(Object* instance)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Instance: {}", PointerToString(instance));

	if (!m_PropertyType->IsStructure())
	{
		return false;
	}

	void* src = (void*)((char*)instance + m_Offset);
	Type::CallCopyConstructor(src, m_Data, m_PropertyType);

	return true;
}

bool PropertyItem::OnWriteObject(Object* instance)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Instance: {}", PointerToString(instance));

	if (!m_PropertyType->IsStructure())
	{
		return false;
	}

	void* dst = (void*)((char*)instance + m_Offset);
	Type::CallCopyConstructor(m_Data, dst, m_PropertyType);

	return true;
}

Object* PropertyItem::GetReadInstance(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return instance;
}

Object* PropertyItem::GetWriteInstance(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return instance;
}
