#include "Config/ConfigSection.h"

#include "Config/ConfigAsset.h"
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
}

Config* ConfigSection::Insert(Config* other)
{
	other->m_Parent = this;
	m_Entries[other->GetName()] = other;
	return other;
}

Config* ConfigSection::Remove(Config* other)
{
	auto it = m_Entries.find(other->GetName());
	if (it != m_Entries.end())
	{
		other->m_Parent = nullptr;

		m_Entries.erase(it);
		return other;
	}

	return nullptr;
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

ConfigSection* ConfigSection::GetBase() const
{
	return m_Base;
}

ConfigAsset* ConfigSection::GetBaseAsset() const
{
	return m_BaseAsset;
}

bool ConfigSection::Deserialize(Lexer* lexer, Config* parent)
{
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
	
	bool isArray = lexer->GetToken() == Tokens::LeftSquareBracket;
	if (lexer->GetToken() != Tokens::LeftCurlyBracket && !isArray)
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

		if (lexer->GetToken() == Tokens::LeftCurlyBracket || lexer->GetToken() == Tokens::LeftSquareBracket)
		{
			ConfigSection* cfgSection = NewObject<ConfigSection>("ConfigSection");
			cfgSection->m_TypeInfo = typeInfo;

			if (!baseLinkPath.empty())
			{
				cfgSection->m_BaseAsset = AssetModule::Get<ConfigAsset>(baseLinkPath);
				cfgSection->m_Base = cfgSection->m_BaseAsset->Get();
			}
			else
			{
				//! TODO:
			}

			Insert(cfgSection);

			if (!cfgSection->Deserialize(lexer, this))
			{
				return false;
			}
		}
		else
		{
			ConfigValue* cfgValue = NewObject<ConfigValue>("ConfigValue");
			cfgValue->m_TypeInfo = typeInfo;

			Insert(cfgValue);
			
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

	if (isArray && lexer->GetToken() != Tokens::RightSquareBracket)
	{
		lexer->Error("Expected ']', got '{}'", lexer->GetTokenValue());
		return false;
	}
	else if (!isArray && lexer->GetToken() != Tokens::RightCurlyBracket)
	{
		lexer->Error("Expected '}', got '{}'", lexer->GetTokenValue());
		return false;
	}
	
	lexer->NextToken();

	return true;
}

bool ConfigSection::Serialize(std::stringstream& output, std::string prefix) const
{
	std::string typeAndName = SerializeTypeAndName();
	bool indent = false;

	if (m_Parent || !typeAndName.empty())
	{
		indent = true;
		output << prefix << typeAndName << (m_BaseAsset ? m_BaseAsset->GetPath() + " " : "") << "{" << std::endl;
		std::cout << std::endl;
	}

	int index = 0;
	for (auto& [k, v] : m_Entries)
	{
		index++;

		if (v == nullptr)
		{
			continue;
		}

		if (!v->Serialize(output, indent ? prefix + " " : prefix))
		{
			continue;
		}

		if (index < m_Entries.size())
		{
			output << ", " << std::endl;
		}
	}

	if (indent)
	{
		output << std::endl << prefix << "}";
	}

	return true;
}
