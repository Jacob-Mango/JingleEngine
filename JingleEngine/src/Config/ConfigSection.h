#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

class ConfigAsset;
class AssetModule;

class ConfigSection : public Config
{
	DEFINE_CLASS(ConfigSection, Config);

	friend Config;
	friend ConfigArray;
	friend ConfigSection;
	friend ConfigValue;

	friend AssetModule;

protected:
	std::map<std::string, Ref<Config>> m_Entries;

	ConfigAsset* m_Base = nullptr; //! TODO: use ConfigAsset

public:
	ConfigSection();
	~ConfigSection();

public:
	virtual Config* Insert(Config* other) override;
	virtual Config* Remove(Config* other) override;

	virtual size_t Count() const override;

	virtual Config* Get(std::string name) const override;
	virtual Config* Get(int index) const override;

	virtual ConfigAsset* GetBase() const override;

	virtual bool Deserialize(JingleScript::Lexer* lexer, Config* parent) override;
	virtual bool Serialize(std::stringstream& output, std::string prefix = "") const override;

};
