#include "Config/ConfigAsset.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(ConfigAsset)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

ConfigAsset::ConfigAsset()
{
	JS_TRACE(Tracers::Asset);

	m_Properties = NewObject<PropertyObject>("PropertyObject");
}

ConfigAsset::~ConfigAsset()
{
	JS_TRACE(Tracers::Asset);

	//delete m_Properties;
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

bool ConfigAsset::WriteToObject(JingleScript::Object* instance)
{
	JS_TRACE(Tracers::Asset);
	JS_TINFO("Instance: {}", PointerToString(instance));

	if (!m_Properties->OnWriteObject(instance))
	{
		return false;
	}

	return true;
}

bool ConfigAsset::ReadFromObject(JingleScript::Object* instance)
{
	JS_TRACE(Tracers::Asset);

	if (!m_Properties->OnReadObject(instance))
	{
		return false;
	}

	if (!m_Properties->OnSerialize(m_Config))
	{
		return false;
	}

	return true;
}
