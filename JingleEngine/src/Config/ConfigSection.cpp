#include "Config/ConfigSection.h"

#include "Config/ConfigAsset.h"
#include "Config/ConfigArray.h"
#include "Config/ConfigValue.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(ConfigSection)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

ConfigSection::ConfigSection()
{
}

ConfigSection::~ConfigSection()
{
	for (auto& [name, entry] : m_Entries)
	{
		//delete entry;
	}
}

void ConfigSection::Add(Config* other)
{
	other->m_Parent = this;
	m_Entries[other->GetName()] = other;
}

void ConfigSection::Remove(Config* other)
{
	auto it = m_Entries.find(other->GetName());
	if (it != m_Entries.end())
	{
		other->m_Parent = nullptr;

		m_Entries.erase(it);
	}
}

size_t ConfigSection::Count() const
{
	return m_Entries.size();
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
		if (!DeserializeTypeAndName(lexer, m_TypeInfo))
		{
			return false;
		}

		if (!GetName().empty())
		{
			lexer->Error("Root sections can't be named.");
			return false;
		}
	}

	if (lexer->GetToken() != Tokens::LeftCurlyBracket)
	{
		lexer->Error("Expected '{', got '{}'", lexer->GetTokenValue());
		return false;
	}

	lexer->NextToken();

	while (true)
	{
		if (lexer->GetToken() == Tokens::RightCurlyBracket)
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
		else if (lexer->GetToken() == Tokens::LeftSquareBracket)
		{
			ConfigArray* cfgArray = NewObject<ConfigArray>("ConfigArray");
			cfgArray->m_TypeInfo = typeInfo;

			if (!cfgArray->Deserialize(lexer, this))
			{
				return false;
			}
		}
		else
		{
			ConfigValue* cfgValue = NewObject<ConfigValue>("ConfigValue");
			cfgValue->m_TypeInfo = typeInfo;
			
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
		lexer->Error("Expected '}', got '{}'", lexer->GetTokenValue());
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
		tempOut << prefix << typeAndName << (m_Base ? m_Base->GetName() + " " : "") << "{" << std::endl;
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
