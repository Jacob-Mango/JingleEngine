#include "Config/ConfigSection.h"

#include "Config/ConfigAsset.h"
#include "Config/ConfigArray.h"
#include "Config/ConfigValue.h"

using namespace JingleScript;

ConfigSection::ConfigSection()
{
}

ConfigSection::~ConfigSection()
{
	for (auto& [name, entry] : m_Entries)
	{
		delete entry;
	}
}

void ConfigSection::Add(Config* other)
{
	other->m_Parent = this;
	m_Entries[other->m_Name] = other;
}

Config* ConfigSection::Get(std::string name) const
{
	Config* entry = nullptr;

	auto it = m_Entries.find(name);
	if (it != m_Entries.end())
	{
		entry = it->second;
	}

	if (!entry && m_Base)
	{
		entry = m_Base->Get(name);
	}

	return entry;
}

Config* ConfigSection::Get(int index) const
{
	auto it = m_Entries.begin();
	std::advance(it, index);
	if (it != m_Entries.end())
		return it->second;

	return nullptr;
}

Config* ConfigSection::GetBase() const
{
	return m_Base;
}

bool ConfigSection::Deserialize(Lexer* lexer)
{
	bool leftCurlyBracket = lexer->GetToken() == Tokens::LeftCurlyBracket;
	if (m_Parent)
	{
		if (!leftCurlyBracket)
		{
			lexer->Error("Expected '{', got '%s'", lexer->GetTokenValue().c_str());
			return false;
		}

		lexer->NextToken();
	}
	else
	{
		std::pair<std::string, std::string> typeAndName;
		if (!DeserializeTypeAndName(lexer, typeAndName, false))
		{
			lexer->Error("Unexpected token '%s'", lexer->GetTokenValue().c_str());
			return false;
		}

		m_CType = typeAndName.first;
		m_Name = typeAndName.second;

		if (!m_Name.empty())
		{
			lexer->Error("Root section can't be named.");
			return false;
		}

		lexer->NextToken();

		bool quotes = lexer->GetTokenType() == TokenType::QUOTE;
		if (quotes)
		{
			std::string value = lexer->GetTokenValue();
			lexer->NextToken();

			m_Base = AssetModule::Get<ConfigAsset>(value);
		}
		
		leftCurlyBracket = lexer->GetToken() == Tokens::LeftCurlyBracket;

		lexer->NextToken();
	}

	bool isFirst = true;
	while (lexer->HasNext())
	{
		if (lexer->GetToken() == Tokens::RightCurlyBracket)
		{
			lexer->NextToken();

			if (lexer->GetToken() == Tokens::Comma)
			{
				lexer->NextToken();
			}

			return true;
		}

		bool wasFirst = isFirst;

		if (!isFirst && lexer->GetToken() != Tokens::Comma)
		{
			lexer->Error("Expected ',', got '%s'", lexer->GetTokenValue().c_str());
			return false;
		}

		if (!isFirst)
		{
			lexer->NextToken();
		}

		isFirst = false;

		std::pair<std::string, std::string> typeAndName;
		if (!DeserializeTypeAndName(lexer, typeAndName))
		{
			return false;
		}

		if (typeAndName.second.empty())
		{
			lexer->Error("Data in sections must be named.");
			return false;
		}

		lexer->NextToken();
		if (lexer->GetToken() == Tokens::LeftCurlyBracket)
		{
			// section

			ConfigSection* cfgSection = new ConfigSection();
			cfgSection->m_CType = typeAndName.first;
			cfgSection->m_Name = typeAndName.second;

			Add(cfgSection);

			if (!cfgSection->Deserialize(lexer))
			{
				return false;
			}
		}
		else if (lexer->GetToken() == Tokens::LeftSquareBracket)
		{
			// array

			ConfigArray* cfgArray = new ConfigArray();
			cfgArray->m_CType = typeAndName.first;
			cfgArray->m_Name = typeAndName.second;

			Add(cfgArray);

			if (!cfgArray->Deserialize(lexer))
			{
				return false;
			}
		}
		else
		{
			bool quotes = lexer->GetTokenType() == TokenType::QUOTE;
			std::string value = lexer->GetTokenValue();
			lexer->NextToken();

			if (lexer->GetToken() == Tokens::LeftCurlyBracket)
			{
				ConfigSection* cfgSection = new ConfigSection();
				cfgSection->m_CType = typeAndName.first;
				cfgSection->m_Name = typeAndName.second;

				Add(cfgSection);

				cfgSection->m_Base = AssetModule::Get<ConfigAsset>(value);

				if (!cfgSection->Deserialize(lexer))
				{
					return false;
				}
			}
			else
			{
				ConfigValue* cfgValue = new ConfigValue();
				cfgValue->m_CType = typeAndName.first;
				cfgValue->m_Name = typeAndName.second;

				Add(cfgValue);

				cfgValue->m_Value = value;
			}
		}
	}

	if (m_Parent)
	{
		lexer->Error("Unexpected end of file.");
		return false;
	}

	return true;
}

bool ConfigSection::Serialize(std::stringstream& output, std::string prefix) const
{
	bool canOutput = m_Parent == nullptr;
	std::stringstream tempOut;

	std::string typeAndName = SerializeTypeAndName();
	bool indent = false;
	if (m_Parent || !typeAndName.empty())
	{
		indent = true;
		tempOut << prefix << typeAndName << (m_Base ? m_Base->m_Name + " " : "") << "{" << std::endl;
	}

	int index = 0;
	for (auto& [k, v] : m_Entries)
	{
		index++;

		if (v == nullptr)
		{
			continue;
		}

		canOutput |= v->Serialize(tempOut, indent ? prefix + " " : prefix);

		if (index < m_Entries.size())
		{
			tempOut << ", " << std::endl;
		}
	}

	if (indent)
	{
		tempOut << std::endl << prefix << "}";
	}

	if (canOutput)
	{
		output << tempOut.str();
	}

	return canOutput;
}
