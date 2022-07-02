#pragma once

#include "Core/Core.h"

#include "Config/ConfigSection.h"
#include "Config/ConfigValue.h"

#include "Property/ObjectProperty.h"

class ConfigAsset : public Asset, public ObjectProperty
{
	SETUP_ASSET(ConfigAsset, Asset)

	friend ConfigSection;
	friend ConfigValue;

private:
	ConfigSection* m_Config;

	std::vector<ConfigSection*> m_Referenced;

public:
	ConfigAsset();
	virtual ~ConfigAsset();

	virtual bool OnLoad() override;
	virtual bool OnSave() override;

	void Output();

	bool OnConfigUpdate(Config* cfg);

	bool Serialize(JingleScript::Object* object);
	bool Deserialize(JingleScript::Object* object);

	ConfigSection* Get() const;

};
