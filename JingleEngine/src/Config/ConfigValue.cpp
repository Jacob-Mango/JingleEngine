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

void ConfigValue::Debug()
{
	JS_TRACE(Tracers::Config);

	JS_TINFO("This: {} {}", GetTypeAndName(), PointerToString(this));
	JS_TINFO("Value: {}", GetValue());
}

bool ConfigValue::Optimize(Config* source, bool isBaseCheck)
{
	JS_TRACE(Tracers::Config);

	JS_TINFO("Source {}", source ? source->ToString() : "null");
	JS_TINFO("Destination {}", ToString());
	JS_TINFO("IsBaseCheck {}", isBaseCheck);

	ConfigValue* src = dynamic_cast<ConfigValue*>(source);
	if (!src)
	{
		// The original was of a different type
		return true;
	}

	if (isBaseCheck)
	{
		bool matches = m_Value.compare(src->m_Value) == 0;
		JS_TINFO("Matches {}", matches);

		return !matches;
	}

	return true;
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
