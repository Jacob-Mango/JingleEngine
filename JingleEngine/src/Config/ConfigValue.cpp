#include "Config/ConfigValue.h"

#include <Compiler/Lexer.h>

ConfigValue::ConfigValue()
{
}

ConfigValue::~ConfigValue()
{
}

void ConfigValue::Serialize(std::stringstream& output, std::string prefix) const
{
	output << prefix << SerializeTypeAndName() << m_Value;
}
