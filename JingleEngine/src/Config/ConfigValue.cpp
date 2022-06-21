#include "Config/ConfigValue.h"

#include <Compiler/Lexer.h>

ConfigValue::ConfigValue()
{
}

ConfigValue::~ConfigValue()
{
}

bool ConfigValue::Serialize(std::stringstream& output, std::string prefix) const
{
	output << prefix << SerializeTypeAndName() << m_Value;
	return true;
}
