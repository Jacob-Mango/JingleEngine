#include "Config/ConfigValue.h"

#include <Compiler/Lexer.h>

using namespace JingleScript;

BEGIN_CLASS_LINK(ConfigValue)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

ConfigValue::ConfigValue()
{
}

ConfigValue::~ConfigValue()
{
}

bool ConfigValue::Deserialize(JingleScript::Lexer* lexer, Config* parent)
{
	if (!Super::Deserialize(lexer, parent))
	{
		return false;
	}

	m_Value = lexer->GetTokenValue();
	lexer->NextToken();

	if (m_Parent)
	{
		m_Parent->Insert(this);
	}

	return true;
}

bool ConfigValue::Serialize(std::stringstream& output, std::string prefix) const
{
	output << prefix << SerializeTypeAndName() << "\"" << m_Value << "\"";
	return true;
}
