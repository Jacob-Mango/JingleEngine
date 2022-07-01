#include "Config/Config.h"

#include "Config/ConfigAsset.h"
#include "Config/ConfigSection.h"
#include "Config/ConfigValue.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

using namespace JingleScript;

BEGIN_CLASS_LINK(Config)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

Config::Config()
{
	m_Parent = nullptr;
}

Config::~Config()
{

}

std::string Config::GetName() const
{
	JS_TRACE(Tracers::Property);

	return m_TypeInfo.m_Name;
}

void Config::SetName(std::string name)
{
	ConfigSection* parent = dynamic_cast<ConfigSection*>(m_Parent);
	if (parent)
	{
		parent->Remove(this);
	}

	m_TypeInfo.m_Name = name;

	if (parent)
	{
		parent->Insert(this);
	}
}

std::string Config::GetLinkedType() const
{
	JS_TRACE(Tracers::Property);

	std::string type;

	const Config* cfg = this;
	while (cfg)
	{
		type = cfg->m_TypeInfo.m_Type;
		cfg = cfg->GetBase() ? cfg->GetBase()->Get() : nullptr;
	}

	return type;
}

void Config::SetLinkedType(std::string type)
{
	m_TypeInfo.m_Type = type;
}

void Config::SetLinkedType(std::string type, bool directly)
{
	m_TypeInfo.m_Type = type;
	m_TypeInfo.m_DirectlyLinked = directly;
}

std::string Config::GetTypeAndName() const
{
	JS_TRACE(Tracers::Property);

	return SerializeTypeAndName();
}

bool Config::IsLinkedDirectly() const
{
	JS_TRACE(Tracers::Property);

	return m_TypeInfo.m_DirectlyLinked;
}

Config* Config::CreateValue(const std::string& name, const std::string& value, JingleScript::Type* type)
{
	Config* cfg = NewObject<ConfigValue>("ConfigValue")->As<Config>();
	cfg->SetName(name);
	if (type)
	{
		cfg->SetLinkedType(type->Name());
	}

	cfg->SetValue(value);
	return Insert(cfg);
}

Config* Config::CreateSection(const std::string& name, JingleScript::Type* type)
{
	Config* cfg = NewObject<ConfigSection>("ConfigSection")->As<Config>();
	cfg->SetName(name);
	if (type)
	{
		cfg->SetLinkedType(type->Name());
	}

	return Insert(cfg);
}

Config* Config::CreateArray(const std::string& name, JingleScript::Type* type)
{
	Config* cfg = NewObject<ConfigArray>("ConfigArray")->As<Config>();
	cfg->SetName(name);
	if (type)
	{
		cfg->SetLinkedType(type->Name());
	}

	return Insert(cfg);
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

bool Config::DeserializeTypeAndName(JingleScript::Lexer* lexer, ConfigTypeInfo& result)
{
	if (lexer->GetTokenType() == TokenType::QUOTE)
	{
		return true;
	}

	if (lexer->GetToken() == Tokens::LeftCurlyBracket)
	{
		lexer->NextToken();

		if (lexer->GetToken() == Tokens::Hash)
		{
			result.m_DirectlyLinked = false;
			lexer->NextToken();
		}

		result.m_Type = lexer->GetTokenValue();
		lexer->NextToken();
		
		if (lexer->GetToken() == Tokens::Colon)
		{
			result.m_Type = "";

			// We aren't deserizaling name or type override
			lexer->PreviousToken();
			lexer->PreviousToken();

			return true;
		}
		else if (lexer->GetToken() == Tokens::Comma)
		{
			result.m_Name = lexer->GetTokenValue();
			lexer->NextToken();

			if (lexer->GetToken() != Tokens::RightCurlyBracket)
			{
				lexer->Error("Expected '}', got '{}'", lexer->GetTokenValue());
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
			lexer->Error("Expected ',' or '}', got '{}'", lexer->GetTokenValue());
			return false;
		}
	}
	else
	{
		result.m_Name = lexer->GetTokenValue();
		lexer->NextToken();
	}

	if (lexer->GetToken() != Tokens::Colon)
	{
		lexer->Error("Expected ':', got '{}'", lexer->GetTokenValue());
		return false;
	}

	lexer->NextToken();
	return true;
}

std::string Config::SerializeTypeAndName() const
{
	const std::string& name = m_TypeInfo.m_Name;
	const std::string& type = m_TypeInfo.m_Type;
	const std::string directlyLinked = m_TypeInfo.m_DirectlyLinked ? "" : "#";

	if (name.empty() && type.empty()) return "";

	if (type.empty()) return name + ": ";

	if (name.empty()) return "{" + directlyLinked + type + "}: ";

	return "{" + directlyLinked + type + ", " + name + "}: ";
}
