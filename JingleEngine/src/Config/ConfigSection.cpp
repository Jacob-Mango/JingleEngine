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

void ConfigSection::Debug()
{
	JS_TRACE(Tracers::Config);

	JS_TINFO("This: {} {}", GetTypeAndName(), PointerToString(this));

	Config* base = GetBase();
	while (base)
	{
		JS_TINFO(" Base: {} {}", base->GetTypeAndName(), PointerToString(base));

		base = base->GetBase();
	}

	for (auto& cfg : *this)
	{
		JS_TINFO("Parent: {}", PointerToString(this));

		cfg.Debug();
	}
}

void ConfigSection::UpdateBase()
{
	if (m_BaseAsset)
	{
		m_Base = m_BaseAsset->Get();
		return;
	}

	m_Base = static_cast<ConfigSection*>(m_Parent);
	m_Base = m_Base ? m_Base->m_Base : nullptr;
	m_Base = m_Base ? dynamic_cast<ConfigSection*>(m_Base->Get(GetName())) : nullptr;

	for (auto& [name, cfg] : m_Entries)
	{
		cfg->UpdateBase();
	}
}

ConfigSection* ConfigSection::GetBase() const
{
	return m_Base;
}

void ConfigSection::SetBaseAsset(ConfigAsset* asset)
{
	m_BaseAsset = asset;
	UpdateBase();
}

ConfigAsset* ConfigSection::GetBaseAsset() const
{
	return m_BaseAsset;
}

//! Should not be used during gameplay
Config* ConfigSection::Set(Config* other)
{
	std::string name = other->GetName();

	auto it = m_Entries.find(name);
	if (it != m_Entries.end())
	{
		if (it->second == other)
		{
			return other;
		}

		//! TODO: find parent that has ConfigAsset and then call 'UpdateBase'
		
		it->second->m_Parent = nullptr;
		it->second->UpdateBase();

		m_Entries.erase(it);
	}
	else
	{
		m_EntryOrder.push_back(name);
	}

	other->m_Parent = this;
	m_Entries[name] = other;

	//! TODO: find parent that has ConfigAsset and then call 'UpdateBase'

	other->UpdateBase();

	return other;
}

//! Should not be used during gameplay
Config* ConfigSection::Remove(Config* other)
{
	std::string name = other->GetName();

	auto it = m_Entries.find(name);
	if (it != m_Entries.end())
	{
		//! TODO: find parent that has ConfigAsset and then call 'UpdateBase'

		other->m_Parent = nullptr;
		other->UpdateBase();

		m_Entries.erase(it);

		m_EntryOrder.erase(std::remove(m_EntryOrder.begin(), m_EntryOrder.end(), name), m_EntryOrder.end());

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

bool ConfigSection::Optimize()
{
	JS_TRACE(Tracers::Config);

	JS_TINFO("This {}", ToString());

	JS_TINFO("BaseAsset {}", m_BaseAsset ? m_BaseAsset->GetPath() : "none");
	JS_TINFO("Base {}", m_Base ? m_Base->ToString() : "null");
		
	auto it = m_Entries.begin();
	while (it != m_Entries.end())
	{
		auto& [name, cfg] = *it;

		JS_NAMED_TRACE(Tracers::Config, name);

		if (cfg->Optimize())
		{
			it++;
			continue;
		}

		cfg->m_Parent = nullptr;
		cfg->UpdateBase();

		m_EntryOrder.erase(std::remove(m_EntryOrder.begin(), m_EntryOrder.end(), name), m_EntryOrder.end());
		it = m_Entries.erase(it);
	}

	if (m_Base)
	{
		if (m_BaseAsset)
		{
			JS_TINFO("BaseAsset");
			return true;
		}

		if (m_TypeInfo.m_Type != m_Base->GetLinkedType())
		{
			JS_TINFO("Type");
			return true;
		}
	}
	else
	{
		// If it isn't an array and the type is set
		if (!IsArray() && !m_TypeInfo.m_Type.empty())
		{
			JS_TINFO("Type");
			return true;
		}
	}

	// If there are still entries then we can't remove this section
	if (m_Entries.size() > 0)
	{
		JS_TINFO("EntryCount");
		return true;
	}

	JS_TINFO("Removing");
	return false;
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

		// Possibly linking to external config as base for section
		if (lexer->GetTokenType() == TokenType::QUOTE)
		{
			std::string baseLinkPath = lexer->GetTokenValue();
			lexer->NextToken();

			if (lexer->GetToken() == Tokens::LeftCurlyBracket)
			{
				m_BaseAsset = AssetModule::Get<ConfigAsset>(baseLinkPath);
				m_BaseAsset->m_Referenced.push_back(this);

				UpdateBase();
			}
			else
			{
				lexer->PreviousToken();
			}
		}
	}

	m_IsArray = lexer->GetToken() == Tokens::LeftSquareBracket;
	if (lexer->GetToken() != Tokens::LeftCurlyBracket && !m_IsArray)
	{
		lexer->Error("Expected '{{', got '{}'", lexer->GetTokenValue());
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
				cfgSection->m_BaseAsset->m_Referenced.push_back(this);
			}

			cfgSection->UpdateBase();

			Set(cfgSection);

			if (!cfgSection->Deserialize(lexer, this))
			{
				return false;
			}
		}
		else
		{
			ConfigValue* cfgValue = NewObject<ConfigValue>("ConfigValue");
			cfgValue->m_TypeInfo = typeInfo;

			Set(cfgValue);
			
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

	if (m_IsArray && lexer->GetToken() != Tokens::RightSquareBracket)
	{
		lexer->Error("Expected ']', got '{}'", lexer->GetTokenValue());
		return false;
	}
	else if (!m_IsArray && lexer->GetToken() != Tokens::RightCurlyBracket)
	{
		lexer->Error("Expected '}}', got '{}'", lexer->GetTokenValue());
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
		output << prefix << typeAndName << (m_BaseAsset ? "\"" + m_BaseAsset->GetPath() + "\" " : "") << (IsArray() ? "[" : "{") << std::endl;
	}

	int index = 0;
	for (auto& name : m_EntryOrder)
	{
		auto& entry = m_Entries.at(name);
		if (!entry)
		{
			continue;
		}

		if (index != 0)
		{
			output << ", " << std::endl;
		}

		entry->Serialize(output, indent ? prefix + " " : prefix);
		index++;
	}

	if (indent)
	{
		output << std::endl << prefix << (IsArray() ? "]" : "}");
	}

	return true;
}
