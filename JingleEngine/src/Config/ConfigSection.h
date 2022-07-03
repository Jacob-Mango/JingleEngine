#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

class ConfigAsset;

class AssetModule;

typedef std::unordered_map<std::string, Ref<Config>> ConfigMap;

class ConfigSection : public Config
{
	DEFINE_CLASS(ConfigSection, Config);

	friend Config;
	friend ConfigSection;
	friend ConfigValue;

	friend ConfigAsset;
	friend ConfigIterator;

	friend AssetModule;

protected:
	ConfigMap m_Entries;

	ConfigSection* m_Base = nullptr;
	ConfigAsset* m_BaseAsset = nullptr;

	bool m_IsArray = false;

public:
	ConfigSection();
	~ConfigSection();

public:
	virtual ConfigIterator begin() { return ConfigIterator(this); }
	virtual ConfigIterator end() { return ConfigIterator(); }

public:
	virtual void Debug() override;

public:
	virtual void SetArray(bool isArray) { m_IsArray = isArray; }
	virtual bool IsArray() const { return m_IsArray; }

	virtual void UpdateBase() override;

	virtual ConfigSection* GetBase() const override;
	
	virtual void SetBaseAsset(ConfigAsset* asset) override;
	virtual ConfigAsset* GetBaseAsset() const override;

	virtual Config* Set(Config* other) override;
	virtual Config* Remove(Config* other) override;

	virtual Config* Get(std::string name) const override;

	virtual bool Optimize() override;

public:
	virtual bool Deserialize(JingleScript::Lexer* lexer, Config* parent) override;
	virtual bool Serialize(std::stringstream& output, std::string prefix = "") const override;

};
