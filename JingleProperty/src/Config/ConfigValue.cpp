#include "Config/ConfigValue.h"

#include <Compiler/Lexer.h>

void ConfigValue::Serialize(std::stringstream& output, std::string prefix) const
{
	output << prefix << SerializeTypeAndName() << m_Value;
}

std::string ConfigValue::ToString() const
{
	std::stringstream ss;
	ss << Super::ToString();

	ss << ", ";
	ss << "Value=" << m_Value;

	return ss.str();
}
