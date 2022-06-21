#include "Config/ConfigSection.h"

#include "Config/ConfigArray.h"
#include "Config/ConfigValue.h"

using namespace JingleScript;

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

		m_Type = typeAndName.first;
		m_Name = typeAndName.second;

		if (!m_Name.empty())
		{
			lexer->Error("Root section can't be named.");
			return false;
		}

		lexer->NextToken();
		
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
			cfgSection->m_Type = typeAndName.first;
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
			cfgArray->m_Type = typeAndName.first;
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
				cfgSection->m_Type = typeAndName.first;
				cfgSection->m_Name = typeAndName.second;

				Add(cfgSection);

				//! TODO: Base will be another file
				cfgSection->m_Base = Get(value);

				if (!cfgSection->Deserialize(lexer))
				{
					return false;
				}
			}
			else
			{
				ConfigValue* cfgValue = new ConfigValue();
				cfgValue->m_Type = typeAndName.first;
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

void ConfigSection::Serialize(std::stringstream& output, std::string prefix) const
{
	std::string typeAndName = SerializeTypeAndName();
	bool indent = false;
	if (m_Parent || !typeAndName.empty())
	{
		indent = true;
		output << prefix << typeAndName << (m_Base ? m_Base->m_Name + " " : "") << "{" << std::endl;
	}

	int index = 0;
	for (auto& [k, v] : m_Entries)
	{
		index++;

		if (v == nullptr)
		{
			continue;
		}

		v->Serialize(output, indent ? prefix + " " : prefix);

		if (index < m_Entries.size())
		{
			output << ", " << std::endl;
		}
	}

	if (indent)
	{
		output << std::endl << prefix << "}";
	}
}

std::string ConfigSection::ToString() const
{
	std::stringstream ss;
	ss << Super::ToString();

	ss << ", ";
	ss << "Count=" << m_Entries.size();

	int index = 0;
	for (auto entry : m_Entries)
	{
		ss << ", ";
		ss << "Entry_" << index++ << "=" << entry.second->AsString();
	}

	return ss.str();
}
