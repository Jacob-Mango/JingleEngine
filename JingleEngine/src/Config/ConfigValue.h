#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

class ConfigSection;

class ConfigValue : public Config
{
	DEFINE_CLASS(ConfigValue, Config);

	friend Config;
	friend ConfigSection;
	friend ConfigValue;

protected:
	std::string m_Value;

public:
	ConfigValue();
	~ConfigValue();

public:
	virtual void Debug() override;

public:
	virtual bool Optimize(Config* source, bool isBaseCheck) override;

public:
	virtual void SetValue(const std::string& value) override { m_Value = value; }
	virtual std::string GetValue() const override { return m_Value; }
	virtual std::string* GetValuePtr() override { return &m_Value; }

	virtual bool Deserialize(JingleScript::Lexer* lexer, Config* parent) override;
	virtual bool Serialize(std::stringstream& output, std::string prefix = "") const override;

};
