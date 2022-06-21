#pragma once

#include "Core/Core.h"

#include "Config/Config.h"

class ConfigSection;

class ConfigValue : public Config
{
	typedef Config Super;

	friend Config;
	friend ConfigArray;
	friend ConfigSection;
	friend ConfigValue;

protected:
	std::string m_Value;

protected:
	ConfigValue();
	ConfigValue(ConfigValue&) = delete;
	ConfigValue(ConfigValue&&) = delete;
	~ConfigValue();

public:
	virtual std::string GetValue() const override { return m_Value; }
	virtual void SetValue(std::string value) override { m_Value = value; }
	virtual std::string* GetValuePtr() override { return &m_Value; }

	virtual bool Serialize(std::stringstream& output, std::string prefix = "") const override;

};
