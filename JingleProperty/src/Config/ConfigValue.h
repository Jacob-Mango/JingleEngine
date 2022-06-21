#pragma once

#include "Config/Config.h"

class ConfigSection;

class ConfigValue : public Config
{
	typedef Config Super;

	friend ConfigSection;

protected:
	std::string m_Value;

public:
	virtual std::string GetValue() const { return m_Value; }

	virtual void Serialize(std::ostringstream& output, std::string prefix = "") const override;

	virtual std::string ToString() const override;

};
