#pragma once

#include "Core/Core.h"

#include "Property/PropertyObject.h"

class Serializable
{
	friend class PropertyObject;

	PropertyObject* m_Properties = nullptr;

public:

	PropertyObject* GetPropertyContainer() const { return m_Properties; }

};
