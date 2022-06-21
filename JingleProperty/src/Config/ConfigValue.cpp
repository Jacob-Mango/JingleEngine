#include "Config/ConfigValue.h"

#include <Compiler/Lexer.h>

void ConfigValue::Serialize(std::ostringstream& output, std::string prefix) const
{
	output << prefix << m_Name << ": " << m_Value << std::endl;
}

std::string ConfigValue::ToString() const
{
	std::stringstream ss;
	ss << Super::ToString();

	ss << ", ";
	ss << "Value=" << m_Value;

	return ss.str();
}
