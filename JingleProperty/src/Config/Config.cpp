#include "Config/Config.h"

#include "Config/ConfigSection.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

bool Config::Deserialize(JingleScript::Lexer* lexer)
{
	return false;
}

void Config::Serialize(std::ostringstream& output, std::string prefix) const
{
}

std::string Config::ToString() const
{
	std::stringstream ss;
	ss << Super::ToString();

	ss << ", ";
	ss << "Name=" << m_Name;

	return ss.str();
}

Config* Config::Load(std::string file)
{
	using namespace JingleScript;

	Ref<Lexer> lexer = Lexer::ParseFile(file);

	if (!lexer->HasNext())
	{
		return nullptr;
	}

	ConfigSection* section = new ConfigSection();
	if (!section->Deserialize(lexer))
	{
		return nullptr;
	}

	return section;
}
