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

ConfigAsset* ConfigSection::GetBaseAsset() const
{
	return m_BaseAsset;
}

//! Should not be used during gameplay
Config* ConfigSection::Set(Config* other)
{
	auto it = m_Entries.find(other->GetName());
	if (it != m_Entries.end())
	{
		if (it->second == other)
		{
			return other;
		}

		//! TODO: find parent that has ConfigAsset and then call 'UpdateBase'
		
		it->second->m_Parent = nullptr;
		m_Entries.erase(it);

		it->second->UpdateBase();
	}

	other->m_Parent = this;
	m_Entries[other->GetName()] = other;

	//! TODO: find parent that has ConfigAsset and then call 'UpdateBase'

	other->UpdateBase();

	return other;
}

//! Should not be used during gameplay
Config* ConfigSection::Remove(Config* other)
{
	auto it = m_Entries.find(other->GetName());
	if (it != m_Entries.end())
	{
		//! TODO: find parent that has ConfigAsset and then call 'UpdateBase'

		other->m_Parent = nullptr;
		m_Entries.erase(it);

		other->UpdateBase();
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

bool ConfigSection::Optimize(Config* source, bool isBaseCheck)
{
	JS_TRACE(Tracers::Config);

	JS_TINFO("Source {}", source ? source->ToString() : "null");
	JS_TINFO("Destination {}", ToString());
	JS_TINFO("IsBaseCheck {}", isBaseCheck);

	ConfigSection* base = nullptr;
	if (source && !isBaseCheck)
	{
		base = dynamic_cast<ConfigSection*>(source->GetBase());

		m_BaseAsset = source->GetBaseAsset();
		UpdateBase();
	}

	JS_TINFO("BaseAsset {}", m_BaseAsset ? m_BaseAsset->GetPath() : "none");
	JS_TINFO("Base {}", base ? base->ToString() : "null");

		
	auto it = m_Entries.begin();
	while (it != m_Entries.end())
	{
		auto& [name, cfg] = *it;

		JS_NAMED_TRACE(Tracers::Config, name);

		bool isInBase = isBaseCheck;

		ConfigSection* baseBase = dynamic_cast<ConfigSection*>(source);
		Config* baseEntry = nullptr;
		while (baseBase && !baseEntry)
		{
			baseEntry = baseBase->m_Entries[name];

			if (!baseEntry)
			{
				isInBase = true;
				baseBase = baseBase->GetBase();
			}

			JS_TINFO("BaseBase {}", baseBase ? baseBase->ToString() : "null");
			JS_TINFO("BaseEntry {}", baseEntry ? baseEntry->ToString() : "null");
		}

		if (cfg->Optimize(baseEntry, isInBase))
		{
			it++;
			continue;
		}

		it = m_Entries.erase(it);
	}

	if (base)
	{
		if (m_BaseAsset)
		{
			JS_TINFO("BaseAsset");
			return true;
		}

		if (m_TypeInfo.m_Type != base->GetLinkedType())
		{
			JS_TINFO("Type");
			return true;
		}
	}
	else if (isBaseCheck)
	{
	}
	else
	{
		// If we are checking the head section with no base and we weren't checking a base to begin with
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
		output << std::endl << prefix << (IsArray() ? "]" : "}");
	}

	return true;
}
