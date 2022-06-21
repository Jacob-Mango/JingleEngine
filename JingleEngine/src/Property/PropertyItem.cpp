#include "Property/PropertyItem.h"

using namespace JingleScript;

PropertyItem::PropertyItem(JingleScript::Type* type, Property* property, uint64_t offset)
	: PropertyBase(type, property), m_Offset(offset), m_Data(nullptr)
{

}

PropertyItem::~PropertyItem()
{

}

bool PropertyItem::OnDeserialize(Config* cfg)
{
	uint64_t size = m_Type->GetReferenceSize();

	m_Data = malloc(size);
	if (!m_Data)
	{
		return false;
	}

	std::string& value = *cfg->GetValuePtr();

	if (m_Type == String::StaticType())
	{
		memcpy(m_Data, &value, size);
		return true;
	}

	FunctionSignature signature;
	signature.Name = "FromString";
	signature.Owner = nullptr;
	signature.ReturnType = m_Type;
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
	return true;
}

void PropertyItem::OnReadObject(JingleScript::Object* instance)
{
}

void PropertyItem::OnWriteObject(JingleScript::Object* instance)
{
	void* dst = (void*)((char*)instance + m_Offset);

	if (m_Type->IsStructure())
	{
		Type::CallCopyConstructor(m_Data, dst, m_Type);
	}
	else
	{
		memcpy(dst, m_Data, sizeof(void*));
	}
}
