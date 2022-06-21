#include "Config/Config.h"

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

bool Config::Deserialize(Lexer* lexer)
{
	return false;
}

void Config::Serialize(std::stringstream& output, std::string prefix) const
{
}

bool Config::DeserializeTypeAndName(Lexer* lexer, std::pair<std::string, std::string>& result, bool checkColon)
{
	std::string tempName = lexer->GetTokenValue();

	//! Handling 'name:'
	if (lexer->GetToken() != Tokens::LeftCurlyBracket)
	{
		result.first = "";
		result.second = "";

		if (checkColon)
		{
			lexer->NextToken();

			if (lexer->GetToken() != Tokens::Colon)
			{
				lexer->Error("Expected ':', got '%s'", lexer->GetTokenValue().c_str());
				return false;
			}
		}

		result.second = tempName;

		return true;
	}

	lexer->NextToken();

	result.first = lexer->GetTokenValue();

	lexer->NextToken();
	
	//! Handling '{ value:'
	if (lexer->GetToken() == Tokens::Colon)
	{
		lexer->PreviousToken();
		lexer->PreviousToken();

		result.first = "";
		result.second = "";

		return true;
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

			return false;
		}

		lexer->PreviousToken();

		return true;
	}

	lexer->NextToken();
	if (lexer->GetToken() != Tokens::Colon)
	{
		lexer->Error("Expected ':', got '%s'", lexer->GetTokenValue().c_str());
		return false;
	}

	return true;
}

std::string Config::SerializeTypeAndName() const
{
	if (m_Name.empty() && m_CType.empty()) return "";

	if (m_CType.empty()) return m_Name + ": ";

	if (m_Name.empty()) return "{" + m_CType + "}: ";

	return "{" + m_CType + ", " + m_Name + "}: ";
}
