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

	if (GetType() != ConfigAsset::StaticType())
	{
		ObjectProperty::Deserialize(m_Config);
	}

	return true;
}

bool ConfigAsset::OnSave()
{
	JS_TRACE(Tracers::Asset);

	if (GetType() != ConfigAsset::StaticType())
	{
		if (!OnConfigUpdate(ObjectProperty::Serialize()))
		{
			JS_ERROR("Failed to update config");
			return false;
		}
	}

	std::string path = GetPath();
	std::ofstream file(path, std::ios::trunc);

	std::stringstream ss;
	m_Config->Serialize(ss);

	file << ss.rdbuf();
	file.close();

	return true;
}

void ConfigAsset::Output()
{
	std::stringstream ss;
	m_Config->Serialize(ss);
	std::cout << ss.str() << std::endl;
}

bool ConfigAsset::OnConfigUpdate(Config* cfgNew)
{
	ConfigSection* cfgRoot = dynamic_cast<ConfigSection*>(cfgNew);
	if (!cfgRoot)
	{
		return false;
	}

	cfgRoot->Optimize(m_Config, false);
	m_Config = cfgRoot;

	//! TODO: all classes that use this asset that aren't directly linked will need to be deserialized

	return true;
}

bool ConfigAsset::Serialize(JingleScript::Object* object)
{
	ObjectProperty* property = dynamic_cast<ObjectProperty*>(object);
	if (!property)
	{
		return false;
	}

	if (!OnConfigUpdate(property->Serialize()))
	{
		return false;
	}

	return true;
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

ConfigSection* ConfigAsset::Get() const
{
	return m_Config;
}
