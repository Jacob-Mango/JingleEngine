#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

class ConfigArray : public Config
{
	typedef Config Super;

	friend Config;
	friend ConfigArray;
	friend ConfigSection;
	friend ConfigValue;

protected:
	std::vector<Config*> m_Entries;

protected:
	ConfigArray();
	ConfigArray(ConfigArray&) = delete;
	ConfigArray(ConfigArray&&) = delete;
	~ConfigArray();

public:
	virtual void Add(Config* other) override;

	virtual size_t Count() const { return m_Entries.size(); }
	virtual Config* Get(int index) const override;

	virtual bool Deserialize(JingleScript::Lexer* lexer) override;
	virtual bool Serialize(std::stringstream& output, std::string prefix = "") const override;

};
