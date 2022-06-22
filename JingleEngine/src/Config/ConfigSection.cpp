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

bool ConfigSection::Deserialize(Lexer* lexer, Config* parent)
{
	if (!Super::Deserialize(lexer, parent))
	{
		return false;
	}

	if (!m_Parent)
	{
		std::pair<std::string, std::string> result;
		if (!DeserializeTypeAndName(lexer, result))
		{
			return false;
		}

		m_CType = result.first;
		m_Name = result.second;

		if (!m_Name.empty())
		{
		lexer->Error("Root sections can't be named.");
		return false;
		}
	}

	if (lexer->GetToken() != Tokens::LeftCurlyBracket)
	{
		lexer->Error("Expected '{', got '%s'", lexer->GetTokenValue().c_str());
		return false;
	}

	lexer->NextToken();

	while (true)
	{
		if (lexer->GetToken() == Tokens::RightCurlyBracket)
		{
			break;
		}

		std::pair<std::string, std::string> result;
		if (!DeserializeTypeAndName(lexer, result))
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
			ConfigSection* cfgSection = new ConfigSection();
			cfgSection->m_CType = result.first;
			cfgSection->m_Name = result.second;

			if (!baseLinkPath.empty())
			{
				cfgSection->m_Base = AssetModule::Get<ConfigAsset>(baseLinkPath);
			}

			if (!cfgSection->Deserialize(lexer, this))
			{
				return false;
			}
		}
		else if (lexer->GetToken() == Tokens::LeftSquareBracket)
		{
			ConfigArray* cfgArray = new ConfigArray();
			cfgArray->m_CType = result.first;
			cfgArray->m_Name = result.second;
			if (!cfgArray->Deserialize(lexer, this))
			{
				return false;
			}
		}
		else
		{
			ConfigValue* cfgValue = new ConfigValue();
			cfgValue->m_CType = result.first;
			cfgValue->m_Name = result.second;
			if (!cfgValue->Deserialize(lexer, this))
			{
				return false;
			}
		}

		if (lexer->GetToken() != Tokens::Comma)
		{
			break;
		}

		lexer->NextToken();
	}

	if (lexer->GetToken() != Tokens::RightCurlyBracket)
	{
		lexer->Error("Expected '}', got '%s'", lexer->GetTokenValue().c_str());
		return false;
	}
	
	lexer->NextToken();

	if (m_Parent)
	{
		m_Parent->Add(this);
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
