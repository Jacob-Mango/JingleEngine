#include "Config/ConfigAsset.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(ConfigAsset)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

ConfigAsset::ConfigAsset()
{
	JS_TRACE(Tracers::Asset);
}

ConfigAsset::~ConfigAsset()
{
	JS_TRACE(Tracers::Asset);
}

bool ConfigAsset::OnLoad()
{
	JS_TRACE(Tracers::Asset);

	return true;
}

void ConfigAsset::Output()
{
	std::stringstream ss;
	m_Config->Serialize(ss);
	std::cout << ss.str() << std::endl;
}

bool ConfigAsset::Serialize()
{
	if (!m_Config->IsLinkedDirectly())
	{
		return false;
	}

	return ObjectProperty::Serialize(m_Config);
}

bool ConfigAsset::Deserialize()
{
	if (!m_Config->IsLinkedDirectly())
	{
		return false;
	}

	return ObjectProperty::Deserialize(m_Config);
}

bool ConfigAsset::Serialize(JingleScript::Object* object)
{
	ObjectProperty* property = dynamic_cast<ObjectProperty*>(object);
	if (!property)
	{
		return false;
	}

	return property->Serialize(m_Config);
}

bool ConfigAsset::Deserialize(JingleScript::Object* object)
{
	ObjectProperty* property = dynamic_cast<ObjectProperty*>(object);
	if (!property)
	{
		return false;
	}

	return property->Deserialize(m_Config);
}
