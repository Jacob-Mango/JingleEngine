#pragma once

#include "Core/Core.h"

#include "Config/ConfigSection.h"

#include "Property/PropertyObject.h"

class ConfigAsset : public Asset, public ConfigSection
{
	SETUP_ASSET(ConfigAsset, Asset)

private:
	PropertyObject* m_Properties;

public:
	ConfigAsset();
	virtual ~ConfigAsset();

	virtual bool OnLoad() override;

	bool WriteToObject(JingleScript::Object* instance);
	bool ReadFromObject(JingleScript::Object* instance);

};
