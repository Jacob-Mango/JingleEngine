#include "Config/ConfigArray.h"

#include "Config/ConfigAsset.h"
#include "Config/ConfigSection.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(ConfigArray)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

ConfigArray::ConfigArray()
{
}

ConfigArray::~ConfigArray()
{
	for (auto& entry : m_Entries)
	{
		//delete entry;
	}
}

void ConfigArray::Add(Config* other)
{
	m_Entries.push_back(other);
}

Config* ConfigArray::Get(int index) const
{
	return m_Entries[index];
}

bool ConfigArray::Deserialize(Lexer* lexer, Config* parent)
{
	if (!Config::Deserialize(lexer, parent))
	{
		return false;
	}

	if (lexer->GetToken() != Tokens::LeftSquareBracket)
	{
		lexer->Error("Expected '[', got '%s'", lexer->GetTokenValue());
		return false;
	}

	lexer->NextToken();

	while (true)
	{
		if (lexer->GetToken() == Tokens::RightSquareBracket)
		{
			break;
		}

		ConfigTypeInfo typeInfo;
		if (!DeserializeTypeAndName(lexer, typeInfo))
		{
			return false;
		}

		std::string baseLinkPath = "";

		// Possibly linking to external config as base for section
		if (lexer->GetTokenType() == TokenType::QUOTE)
		{
			baseLinkPath = lexer->GetTokenValue();
			lexer->NextToken();

			if (lexer->GetToken() != Tokens::LeftCurlyBracket)
			{
				lexer->PreviousToken();
				baseLinkPath = "";
			}
		}

		if (lexer->GetToken() == Tokens::LeftCurlyBracket)
		{
			ConfigSection* cfgSection = NewObject<ConfigSection>("ConfigSection");
			cfgSection->m_TypeInfo = typeInfo;

			if (!baseLinkPath.empty())
			{
				cfgSection->m_Base = AssetModule::Get<ConfigAsset>(baseLinkPath)->m_Config;
			}

			if (!cfgSection->Deserialize(lexer, this))
			{
				return false;
			}
		}
		else
		{
			//! TODO: 
		}

		if (lexer->GetToken() != Tokens::Comma)
		{
			break;
		}

		lexer->NextToken();
	}

	if (lexer->GetToken() != Tokens::RightSquareBracket)
	{
		lexer->Error("Expected ']', got '%s'", lexer->GetTokenValue());
		return false;
	}
	
	lexer->NextToken();

	if (m_Parent)
	{
		m_Parent->Add(this);
	}

	return true;
}

bool ConfigArray::Serialize(std::stringstream& output, std::string prefix) const
{
	bool canOutput = false;
	std::stringstream tempOut;

	tempOut << prefix << SerializeTypeAndName() << "[" << std::endl;

	int index = 0;
	for (auto& v : m_Entries)
	{
		index++;

		if (v == nullptr)
		{
			continue;
		}

		canOutput |= v->Serialize(tempOut, prefix + " ");

		if (index < m_Entries.size())
		{
			tempOut << ", " << std::endl;
		}
	}

	tempOut << std::endl << prefix << "]";

	if (canOutput)
	{
		output << tempOut.str();
	}

	return canOutput;
}
