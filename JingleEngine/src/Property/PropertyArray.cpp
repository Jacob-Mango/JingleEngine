#include "Property/PropertyArray.h"

#include "Property/PropertyItem.h"
#include "Property/PropertyObject.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(PropertyArray)
	LINK_CONSTRUCTOR(Type*, Property*, uint64_t);
END_CLASS_LINK()

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
	JS_TINFO("Deserializing {}", cfg->GetTypeAndName());

	m_Properties.clear();

	int count = cfg->Count();
	for (int i = 0; i < count; i++)
	{
		Config* cfgVariable = cfg->Get(i);
		if (!cfgVariable)
		{
			continue;
		}

		std::string typeStr = cfgVariable->GetLinkedType();
		PropertyBase* propertyData = m_Property->CreateContainerDefault(typeStr, i);
		if (!propertyData)
		{
			JS_TINFO("Couldn't create container for type '{}'.", typeStr);
			return false;
		}

		if (!propertyData->OnDeserialize(cfgVariable))
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
	JS_TINFO("Instance: {}", PointerToString(instance));

	return true;
}

bool PropertyArray::OnWriteObject(Object* instance)
{
	JS_TRACE(Tracers::Property);
	JS_TINFO("Instance: {}", PointerToString(instance));

	FunctionSignature signature;
	signature.Name = "Insert";
	signature.Owner = m_Property->GetOwner();
	signature.ReturnType = nullptr;
	signature.Parameters.push_back({ Object::StaticType() });

	auto function = signature.Find();

	if (!function)
	{
		std::cerr << "Could not find 'Insert' method in " << m_PropertyType->Name() << std::endl;
		return false;
	}

	uint64_t size = sizeof(std::vector<Object*>);

	Thread* thread = Thread::Current();
	ValueStack* stack = &thread->Stack;

	void* data = (void*)((char*)instance + m_Offset);

	ValueData dst;
	dst.Size = size;
	dst.Offset = size;
	dst.IsPointer = !m_PropertyType->IsStructure();

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

size_t PropertyArray::Count() const
{
	return m_Properties.size();
}

PropertyBase* PropertyArray::Get(size_t index) const
{
	return m_Properties[index];
}
