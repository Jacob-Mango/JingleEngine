#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

class ConfigAsset;

class ConfigSection : public Config
{
	typedef Config Super;

	friend Config;
	friend ConfigArray;
	friend ConfigSection;
	friend ConfigValue;

protected:
	std::map<std::string, Config*> m_Entries;

	Ref<ConfigAsset> m_Base;

protected:
	ConfigSection();
	ConfigSection(ConfigSection&) = delete;
	ConfigSection(ConfigSection&&) = delete;
	~ConfigSection();

public:
	virtual void Add(Config* other) override;

	virtual size_t Count() const { return m_Entries.size(); }

	virtual Config* Get(std::string name) const override;
	virtual Config* Get(int index) const override;

	virtual Config* GetBase() const override;

	virtual bool Deserialize(JingleScript::Lexer* lexer) override;
	virtual bool Serialize(std::stringstream& output, std::string prefix = "") const override;

};
