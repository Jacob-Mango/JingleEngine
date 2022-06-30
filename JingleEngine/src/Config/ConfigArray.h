#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

class ConfigArray : public Config
{
	DEFINE_CLASS(ConfigArray, Config);

	friend Config;
	friend ConfigArray;
	friend ConfigSection;
	friend ConfigValue;

protected:
	std::vector<Config*> m_Entries;

public:
	ConfigArray();
	~ConfigArray();

public:
	virtual void Add(Config* other) override;
	virtual void Remove(Config* other) override;

	virtual size_t Count() const override;
	
	virtual Config* Get(int index) const override;

	virtual bool Deserialize(JingleScript::Lexer* lexer, Config* parent) override;
	virtual bool Serialize(std::stringstream& output, std::string prefix = "") const override;

};
