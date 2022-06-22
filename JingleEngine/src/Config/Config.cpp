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

bool Config::Deserialize(Lexer* lexer, Config* parent)
{
	m_Parent = parent;
	return true;
}

bool Config::Serialize(std::stringstream& output, std::string prefix) const
{
	return true;
}

bool Config::DeserializeTypeAndName(JingleScript::Lexer* lexer, std::pair<std::string, std::string>& result)
{
	if (lexer->GetTokenType() == TokenType::QUOTE)
	{
		return true;
	}

	if (lexer->GetToken() == Tokens::LeftCurlyBracket)
	{
		lexer->NextToken();

		result.first = lexer->GetTokenValue();
		lexer->NextToken();
		
		if (lexer->GetToken() == Tokens::Colon)
		{
			result.first = "";

			// We aren't deserizaling name or type override
			lexer->PreviousToken();
			lexer->PreviousToken();

			return true;
		}
		else if (lexer->GetToken() == Tokens::Comma)
		{
			result.second = lexer->GetTokenValue();
			lexer->NextToken();

			if (lexer->GetToken() != Tokens::RightCurlyBracket)
			{
				lexer->Error("Expected '}', got '%s'", lexer->GetTokenValue().c_str());
				return false;
			}

			lexer->NextToken();
		}
		else if (lexer->GetToken() == Tokens::RightCurlyBracket)
		{
			lexer->NextToken();
		}
		else
		{
			lexer->Error("Expected ',' or '}', got '%s'", lexer->GetTokenValue().c_str());
			return false;
		}
	}
	else
	{
		result.second = lexer->GetTokenValue();
		lexer->NextToken();
	}

	if (lexer->GetToken() != Tokens::Colon)
	{
		lexer->Error("Expected ':', got '%s'", lexer->GetTokenValue().c_str());
		return false;
	}

	lexer->NextToken();
	return true;
}

std::string Config::SerializeTypeAndName() const
{
	if (m_Name.empty() && m_CType.empty()) return "";

	if (m_CType.empty()) return m_Name + ": ";

	if (m_Name.empty()) return "{" + m_CType + "}: ";

	return "{" + m_CType + ", " + m_Name + "}: ";
}
