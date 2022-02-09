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

bool IsLetterOrDigit(char c)
{
	return std::isalpha(c) || std::isdigit(c);
}

bool IsWhiteSpace(char c)
{
	return std::isspace(c);
}

void SkipLine(std::ifstream& stream)
{
	char c;
	stream >> c;
	while (c != '\n' && !stream.eof())
	{
		stream >> c;
	}

	stream.seekg(-1, stream.cur);
}

void SkipComment(std::ifstream& stream)
{
	char c;
	stream >> c;
	while (!stream.eof())
	{
		while (c != '*' && !stream.eof())
			stream >> c;

		stream >> c;
		if (c == '/')
			return;
	}
}

char GetChar(std::ifstream& stream)
{
	if (stream.eof())
		return -1;

	char c;
	stream >> c;

	while (!stream.eof())
	{
		if (c == '/')
		{
			stream >> c;
			if (c == '/')
			{
				SkipLine(stream);
			}
			else if (c == '*')
			{
				SkipComment(stream);
			}
			else
			{
				stream.seekg(-1, stream.cur);
				return '/';
			}
		}
		else
		{
			break;
		}

		stream >> c;
	}

	return c;
}

char GetCharWS(std::ifstream& stream)
{
	char c = GetChar(stream);
	while (!stream.eof() && IsWhiteSpace(c))
		c = GetChar(stream);

	return c;
}

std::string GetWord(std::ifstream& stream)
{
	std::string word = "";

	char c = GetCharWS(stream);

	while (!stream.eof() && (IsLetterOrDigit((char)c) || c == '_'))
	{
		word += c;
		c = GetChar(stream);
	}

	stream.seekg(-1, stream.cur);

	return word;
}

std::string GetWord(std::ifstream& stream, bool& quoted)
{
	std::string word = "";

	char preC = 0;
	char c = GetCharWS(stream);

	quoted = false;
	if (c == '"')
	{
		c = GetChar(stream);
		quoted = true;
	}

	if (quoted)
	{
		while (!stream.eof())
		{
			if (c == '"' && preC != '\\')
			{
				c = GetChar(stream);
				break;
			}

			word += c;
			preC = c;
			c = GetChar(stream);
		}
	}
	else
	{
		int charNum = 0;
		while (!stream.eof() && (IsLetterOrDigit((char)c) || c == '_' || c == '.' || (charNum == 0 && c == '-')))
		{
			charNum++;
			word += (char)c;
			c = GetChar(stream);
		}
	}

	stream.seekg(-1, stream.cur);

	return word;
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

bool Config::Load(std::ifstream& stream)
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

bool ConfigArray::Load(std::ifstream& stream)
{
	while (true)
	{
		char c = GetCharWS(stream);

		if (c == ']')
		{
			c = GetCharWS(stream);

			if (c == ',')
			{
				c = GetCharWS(stream);
			}

			stream.seekg(-1, stream.cur);

			return true;
		}

		if (c != '{')
		{
			std::cerr << "Expected '{', got '" << c << "'" << std::endl;
			return false;
		}

		// section

		Config* entry = new ConfigSection();
		entry->m_Name = "";
		if (!entry->Load(stream))
		{
			std::cout << "Failed to read array entry" << std::endl;
			return false;
		}

		if (entry != nullptr)
		{
			Add(entry);
		}
	}
}

bool ConfigSection::Load(std::ifstream& stream)
{
	while (true)
	{
		char c = GetCharWS(stream);

		if (c == '}')
		{
			c = GetCharWS(stream);

			if (c == ',')
			{
				c = GetCharWS(stream);
			}

			stream.seekg(-1, stream.cur);

			return true;
		}

		stream.seekg(-1, stream.cur);

		Config* entry = nullptr;

		std::string name = GetWord(stream);
		c = GetCharWS(stream);

		if (c != ':')
		{
			std::cerr << "Expected ':', got '" << c << "'" << std::endl;
			return false;
		}

		c = GetCharWS(stream);
		if (c == '{')
		{
			// section

			entry = new ConfigSection();
			entry->m_Name = name;

			if (!entry->Load(stream))
			{
				std::cout << "Failed to read section entry" << std::endl;
				return false;
			}
		}
		else if (c == '[')
		{
			// array

			entry = new ConfigArray();
			entry->m_Name = name;

			if (!entry->Load(stream))
			{
				std::cout << "Failed to read array" << std::endl;
				return false;
			}
		}
		else
		{
			// value
			stream.seekg(-1, stream.cur);

			bool quotes = false;
			std::string value = GetWord(stream, quotes);
			c = GetCharWS(stream);

			if (c == '{')
			{

				entry = new ConfigSection();
				entry->m_Name = name;
				static_cast<ConfigSection*>(entry)->m_Base = Get(value);

				if (!entry->Load(stream))
				{
					std::cout << "Failed to read section entry" << std::endl;
					return false;
				}
			}
			else
			{
				if (c != ',')
				{
					std::cerr << "Expected ',', got '" << c << "'" << std::endl;
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

	std::ifstream stream(file);
	if (stream.is_open())
	{
		stream >> std::noskipws;

		char c = GetCharWS(stream);
		if (c != '{')
			return g_NullConfig;

		ConfigSection* section = new ConfigSection();
		if (section->Load(stream))
		{
			return section;
		}
	}

	return g_NullConfig;
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
	super::ToString(output, prefix);

	output << prefix << m_Name << ": " << String << std::endl;
}

void ConfigArray::ToString(std::ostringstream& output, std::string prefix)
{
	super::ToString(output, prefix);

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
	super::ToString(output, prefix);
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

std::string Config::ToString()
{
	std::stringstream ss;
	ss << super::ToString();

	ss << ", ";
	ss << "Name=" << m_Name;

	return ss.str();
}

std::string ConfigNull::ToString()
{
	std::stringstream ss;
	ss << super::ToString();

	ss << ", ";
	ss << "NULL";

	return ss.str();
}

std::string ConfigValue::ToString()
{
	std::stringstream ss;
	ss << super::ToString();

	ss << ", ";
	ss << "Int=" << Int;

	ss << ", ";
	ss << "Float=" << Float;

	ss << ", ";
	ss << "String=" << String;

	return ss.str();
}


std::string ConfigArray::ToString()
{
	std::stringstream ss;
	ss << super::ToString();

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


std::string ConfigSection::ToString()
{
	std::stringstream ss;
	ss << super::ToString();

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
