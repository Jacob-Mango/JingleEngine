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
	JS_TINFO("Deserializing {}", cfg ? cfg->GetTypeAndName() : "null");

	m_Properties.clear();

	if (!cfg)
	{
		return true;
	}

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

		if (!propertyData->Deserialize(cfgVariable, this))
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
	signature.Parameters.push_back({ m_PropertyType });

	bool useOwner = false;
	
	ArrayProperty* arrayProperty = dynamic_cast<ArrayProperty*>(m_Property);
	if (arrayProperty)
	{
		signature.Name = arrayProperty->m_InsertFunction;

		if (arrayProperty->m_UseInstanceInsert)
		{
			signature.Owner = instance->GetType();
			useOwner = true;
		}
	}

	auto function = signature.Find();

	if (!function)
	{
		JS_ERROR("Could not find '{}'.", signature.ToString());
		return false;
	}

	Thread* thread = Thread::Current();
	ValueStack* stack = &thread->Stack;

	uint64_t size = signature.Owner->GetReferenceSize();

	ValueData dst;
	dst.Size = size;
	dst.Offset = size;

	stack->Push(size);
	
	void* data = nullptr;
	if (useOwner)
	{
		data = (void*)instance;
		dst.IsPointer = true;

		stack->CopyFrom(dst, &data);
	}
	else
	{
		data = (void*)((char*)instance + m_Offset);
		dst.IsPointer = false;

		stack->CopyFrom(dst, data);
	}

	for (auto& property : m_Properties)
	{
		//! TODO: check to see if an instance exists at the index

		Object* write = property->GetPropertyType()->New<Object>();
		
		if (property->OnWriteObject(write))
		{
			uint64_t offset = 0;
			Function_Parameter(stack, write, offset);

			function->Call(thread);

			stack->Pop(offset);
		}
	}

	if (!useOwner)
	{
		stack->CopyTo(data, dst);
	}

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
