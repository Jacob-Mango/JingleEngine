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
	std::map<std::string, Config*> m_Entries;

	//Ref<ConfigAsset> m_Base; //! TODO: use ConfigAsset
	Config* m_Base = nullptr;

public:
	ConfigSection();
	~ConfigSection();

public:
	virtual void Add(Config* other) override;

	virtual size_t Count() const override { return m_Entries.size(); }

	virtual Config* Get(std::string name) const override;
	virtual Config* Get(int index) const override;

	virtual Config* GetBase() const override;

	virtual bool Deserialize(JingleScript::Lexer* lexer, Config* parent) override;
	virtual bool Serialize(std::stringstream& output, std::string prefix = "") const override;

};
