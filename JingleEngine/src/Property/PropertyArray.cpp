#include "Property/PropertyArray.h"

#include "Property/PropertyItem.h"
#include "Property/PropertyObject.h"

using namespace JingleScript;

PropertyArray::PropertyArray(Type* type, Property* property, uint64_t offset)
	: PropertyBase(type, property), m_Offset(offset)
{
	JS_TRACE(Tracers::Property);

}

PropertyArray::~PropertyArray()
{
	JS_TRACE(Tracers::Property);

}

bool PropertyArray::OnDeserialize(Config* cfg)
{
	JS_TRACE(Tracers::Property);

	m_Properties.clear();

	ArrayProperty* arrayProp = dynamic_cast<ArrayProperty*>(m_Property);
	if (arrayProp == nullptr)
	{
		return false;
	}

	Type* baseType = arrayProp->m_TemplateType;

	int count = cfg->Count();
	for (int i = 0; i < count; i++)
	{
		Config* cfgVariable = cfg->Get(i);
		if (!cfgVariable)
		{
			continue;
		}
		std::string typeStr = ""; //cfgVariable->GetLinkedType();

		PropertyBase* propertyData = arrayProp->CreateContainer(typeStr, i);
		if (!propertyData || !propertyData->OnDeserialize(cfgVariable))
		{
			return false;
		}

		m_Properties.push_back(propertyData);
    }

    return true;
}

bool PropertyArray::OnSerialize(Config* cfg)
{
	JS_TRACE(Tracers::Property);

    return true;
}

bool PropertyArray::OnReadObject(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return true;
}

bool PropertyArray::OnWriteObject(Object* instance)
{
	JS_TRACE(Tracers::Property);

	FunctionSignature signature;
	signature.Name = "Insert";
	signature.Owner = m_Type;
	signature.ReturnType = nullptr;
	signature.Parameters.push_back({ Object::StaticType() });

	auto function = signature.Find();

	if (!function)
	{
		std::cerr << "Could not find 'Insert' method in " << m_Type->Name() << std::endl;
		return false;
	}

	uint64_t size = m_Type->GetReferenceSize();

	Thread* thread = Thread::Current();
	ValueStack* stack = &thread->Stack;

	void* data = (void*)((char*)instance + m_Offset);

	ValueData dst;
	dst.Size = size;
	dst.Offset = size;
	dst.IsPointer = !m_Type->IsStructure();

	stack->Push(size);
	stack->CopyFrom(dst, data);

	for (auto& property : m_Properties)
	{
		Object* write = property->GetWriteInstance(instance);
		
		if (!property->OnWriteObject(write))
		{
			return false;
		}

		uint64_t offset = 0;
		Function_Parameter(stack, write, offset);

		function->Call(thread);

		stack->Pop(offset);
	}

	stack->CopyTo(data, dst);

	stack->Pop(size);

	return true;
}

Object* PropertyArray::GetReadInstance(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return instance;
}

Object* PropertyArray::GetWriteInstance(Object* instance)
{
	JS_TRACE(Tracers::Property);

	return instance;
}
