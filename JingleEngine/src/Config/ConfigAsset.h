#pragma once

#include "Core/Core.h"

#include "Config/ConfigSection.h"
#include "Config/ConfigArray.h"

#include "Property/PropertyObject.h"

class ConfigAsset : public Asset
{
	SETUP_ASSET(ConfigAsset, Asset)

	friend ConfigArray;
	friend ConfigSection;

private:
	ConfigSection* m_Config;
	PropertyObject* m_Properties;

public:
	ConfigAsset();
	virtual ~ConfigAsset();

	virtual bool OnLoad() override;

	void Output();

	JingleScript::Object* Create();

	bool WriteToObject(JingleScript::Object* instance);
	bool ReadFromObject(JingleScript::Object* instance);

};
