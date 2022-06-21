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
	virtual std::string GetValue() const { return m_Value; }

	virtual void Serialize(std::stringstream& output, std::string prefix = "") const override;

};
