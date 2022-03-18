#include "Config.h"

#include <cctype>

#include <iostream>
#include <string>
#include <fstream>

Config* g_NullConfig = nullptr;

void CheckNullConfig()
{
	if (g_NullConfig == nullptr)
	{
		g_NullConfig = new ConfigNull();
	}
}

Config::Config()
{
}

Config::~Config()
{
}

void Config::Add(Config* other)
{
}

Config* Config::Get(std::string name)
{
	CheckNullConfig();

	return nullptr;
}

Config* Config::Get(int index)
{
	CheckNullConfig();

	return nullptr;
}

std::string Config::GetName()
{
	return m_Name;
}
Config* Config::GetBase()
{
	return nullptr;
}
Config* Config::GetParent()
{
	return m_Parent;
}

Config& Config::operator[](std::string name)
{
	CheckNullConfig();

	return *Get(name);
}

Config& Config::operator[](int index)
{
	CheckNullConfig();

	return *Get(index);
}

void ConfigSection::Add(Config* other)
{
	other->m_Parent = this;
	m_Entries[other->m_Name] = other;

	Count++;
}

Config* ConfigSection::Get(std::string name)
{
	CheckNullConfig();

	Config* entry = m_Entries[name];

	if (!entry && m_Base)
		entry = m_Base->Get(name);

	if (!entry)
		return g_NullConfig;

	return entry;
}

Config* ConfigSection::Get(int index)
{
	CheckNullConfig();

	auto it = m_Entries.begin();
	std::advance(it, index);
	if (it != m_Entries.end())
		return it->second;

	return g_NullConfig;
}

Config* ConfigSection::GetBase()
{
	return m_Base;
}

bool Config::Load(JingleScript::Lexer* lexer)
{
	return false;
}

void ConfigArray::Add(Config* other)
{
	other->m_Parent = this;
	m_Entries.push_back(other);

	Count++;
}

Config* ConfigArray::Get(int index)
{
	CheckNullConfig();

	return m_Entries[index];
}

bool ConfigArray::Load(JingleScript::Lexer* lexer)
{
	using namespace JingleScript;

	while (true)
	{
		if (lexer->GetToken() == Tokens::RightSquareBracket)
		{
			lexer->NextToken();

			if (lexer->GetToken() == Tokens::Comma)
			{
				lexer->NextToken();
			}

			return true;
		}

		if (lexer->GetToken() != Tokens::LeftCurlyBracket)
		{
			lexer->Error("Expected '{', got '%s'", lexer->GetTokenValue().c_str());
			return false;
		}

		// section

		Config* entry = new ConfigSection();
		entry->m_Name = "";
		if (!entry->Load(lexer))
		{
			return false;
		}

		if (entry != nullptr)
		{
			Add(entry);
		}
	}
}

bool ConfigSection::Load(JingleScript::Lexer* lexer)
{
	using namespace JingleScript;

	while (true)
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

		Config* entry = nullptr;

		std::string name = lexer->GetTokenValue();
		lexer->NextToken();

		if (lexer->GetToken() != Tokens::Colon)
		{
			lexer->Error("Expected ':', got '%s'", lexer->GetTokenValue().c_str());
			return false;
		}

		lexer->NextToken();
		if (lexer->GetToken() == Tokens::LeftCurlyBracket)
		{
			// section

			entry = new ConfigSection();
			entry->m_Name = name;

			if (!entry->Load(lexer))
			{
				return false;
			}
		}
		else if (lexer->GetToken() == Tokens::LeftSquareBracket)
		{
			// array

			entry = new ConfigArray();
			entry->m_Name = name;

			if (!entry->Load(lexer))
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
				entry = new ConfigSection();
				entry->m_Name = name;
				static_cast<ConfigSection*>(entry)->m_Base = Get(value);

				if (!entry->Load(lexer))
				{
					return false;
				}
			}
			else
			{
				if (lexer->GetToken() != Tokens::Comma)
				{
					lexer->Error("Expected ',', got '%s'", lexer->GetTokenValue().c_str());
					return false;
				}

				entry = new ConfigValue();
				entry->m_Name = name;

				entry->String = value;

				if (std::strcmp(value.c_str(), "false") == 0)
				{
					entry->Int = 0;
				}
				else if (std::strcmp(value.c_str(), "true") == 0)
				{
					entry->Int = 1;
				}
				else if (!quotes)
				{
					std::string::size_type size;
					entry->Float = std::stof(value, &size);
					entry->Int = std::stoi(value, &size);
				}
			}
		}

		if (entry != nullptr)
		{
			Add(entry);
		}
	}
}

Config* Config::Load(std::string file)
{
	CheckNullConfig();

	Ref<JingleScript::Lexer> lexer = JingleScript::Lexer::ParseFile(file);

	if (!lexer->HasNext())
		return g_NullConfig;

	ConfigSection* section = new ConfigSection();
	if (!section->Load(lexer))
		return g_NullConfig;

	return section;
}

void Config::ToString(std::ostringstream& output, std::string prefix)
{
	//output << prefix << "UNKNOWN ENTRY" << std::endl;
}

void ConfigNull::ToString(std::ostringstream& output, std::string prefix)
{
	output << prefix << "NULL ENTRY" << std::endl;
}

void ConfigValue::ToString(std::ostringstream& output, std::string prefix)
{
	Super::ToString(output, prefix);

	output << prefix << m_Name << ": " << String << std::endl;
}

void ConfigArray::ToString(std::ostringstream& output, std::string prefix)
{
	Super::ToString(output, prefix);

	output << prefix << m_Name << ": " << Int << std::endl;

	for (auto& v : m_Entries)
	{
		if (v == nullptr)
			continue;

		v->ToString(output, prefix + " ");
	}
}

void ConfigSection::ToString(std::ostringstream& output, std::string prefix)
{
	Super::ToString(output, prefix);
	output << prefix << m_Name << ": " << (m_Base ? m_Base->m_Name : "") << std::endl;

	for (auto& [k, v] : m_Entries)
	{
		if (v == nullptr)
			continue;

		v->ToString(output, prefix + " ");
	}
}

bool Config::IsNull()
{
	return false;
}

bool ConfigNull::IsNull()
{
	return true;
}

std::string Config::ToString() const
{
	std::stringstream ss;
	ss << Super::ToString();

	ss << ", ";
	ss << "Name=" << m_Name;

	return ss.str();
}

std::string ConfigNull::ToString() const
{
	std::stringstream ss;
	ss << Super::ToString();

	ss << ", ";
	ss << "NULL";

	return ss.str();
}

std::string ConfigValue::ToString() const
{
	std::stringstream ss;
	ss << Super::ToString();

	ss << ", ";
	ss << "Int=" << Int;

	ss << ", ";
	ss << "Float=" << Float;

	ss << ", ";
	ss << "String=" << String;

	return ss.str();
}


std::string ConfigArray::ToString() const
{
	std::stringstream ss;
	ss << Super::ToString();

	ss << ", ";
	ss << "Count=" << Count;

	int index = 0;
	for (auto entry : m_Entries)
	{
		ss << ", ";
		ss << "Entry_" << index++ << "=" << entry->AsString();
		index++;
	}

	return ss.str();
}


std::string ConfigSection::ToString() const
{
	std::stringstream ss;
	ss << Super::ToString();

	ss << ", ";
	ss << "Count=" << Count;

	int index = 0;
	for (auto entry : m_Entries)
	{
		ss << ", ";
		ss << "Entry_" << index++ << "=" << entry.second->AsString();
	}

	return ss.str();
}
