#include "Config/ConfigValue.h"

#include "Config/ConfigSection.h"

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

void ConfigValue::Debug()
{
	JS_TRACE(Tracers::Config);

	JS_TINFO("This: {} {}", GetTypeAndName(), PointerToString(this));
	JS_TINFO("Value: {}", GetValue());
}

bool ConfigValue::Optimize()
{
	JS_TRACE(Tracers::Config);

	JS_TINFO("This {}", ToString());

	Config* parent = GetParent();
	if (!parent)
	{
		return true;
	}

	ConfigSection* parentBase = parent->GetBase();
	if (!parentBase)
	{
		return true;
	}

	if (m_Value != parentBase->GetValue(GetName()))
	{
		return true;
	}

	return false;
}

bool ConfigValue::Deserialize(JingleScript::Lexer* lexer, Config* parent)
{
	m_Value = lexer->GetTokenValue();
	lexer->NextToken();

	return true;
}

bool ConfigValue::Serialize(std::stringstream& output, std::string prefix) const
{
	output << prefix << SerializeTypeAndName() << "\"" << m_Value << "\"";
	return true;
}
