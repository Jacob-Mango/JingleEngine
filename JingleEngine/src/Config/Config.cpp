#include "Config/Config.h"

#include "Config/ConfigAsset.h"
#include "Config/ConfigSection.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

using namespace JingleScript;

Config::Config()
{
	m_Parent = nullptr;
}

Config::~Config()
{

}

std::string Config::GetName() const
{
	return m_Name;
}

std::string Config::GetType() const
{
	std::string type;

	const Config* cfg = this;
	while (cfg)
	{
		type = cfg->m_CType;
		cfg = cfg->GetBase();
	}

	return type;
}

bool Config::Deserialize(Lexer* lexer)
{
	return false;
}

bool Config::Serialize(std::stringstream& output, std::string prefix) const
{
	return true;
}

int Config::DeserializeTypeAndName(Lexer* lexer, std::pair<std::string, std::string>& result, bool checkColon)
{
	std::string tempName = lexer->GetTokenValue();

	//! Handling 'name:'
	if (lexer->GetToken() != Tokens::LeftCurlyBracket)
	{
		result.first = "";
		result.second = "";

		lexer->NextToken();

		if (checkColon)
		{
			if (lexer->GetToken() != Tokens::Colon)
			{
				lexer->Error("Expected ':', got '%s'", lexer->GetTokenValue().c_str());
				return 0;
			}

			lexer->NextToken();
		}

		result.second = tempName;

		return 1;
	}

	lexer->NextToken();

	result.first = lexer->GetTokenValue();

	lexer->NextToken();
	
	//! Handling '{ value:'
	if (lexer->GetToken() == Tokens::Colon)
	{
		lexer->NextToken();

		result.second = result.first;
		result.first = "";

		return 2;
	}

	bool isComma = lexer->GetToken() == Tokens::Comma;
	if (isComma)
	{
		lexer->NextToken();

		result.second = lexer->GetTokenValue();

		lexer->NextToken();
	}

	if (lexer->GetToken() != Tokens::RightCurlyBracket)
	{
		lexer->PreviousToken();
		lexer->PreviousToken();

		if (isComma)
		{
			lexer->Error("Expected ',', got '%s'", lexer->GetTokenValue().c_str());

			return 0;
		}

		lexer->PreviousToken();

		return 1;
	}

	lexer->NextToken();
	if (lexer->GetToken() != Tokens::Colon)
	{
		lexer->Error("Expected ':', got '%s'", lexer->GetTokenValue().c_str());
		return 0;
	}

	lexer->NextToken();

	return 1;
}

std::string Config::SerializeTypeAndName() const
{
	if (m_Name.empty() && m_CType.empty()) return "";

	if (m_CType.empty()) return m_Name + ": ";

	if (m_Name.empty()) return "{" + m_CType + "}: ";

	return "{" + m_CType + ", " + m_Name + "}: ";
}
