#include "Config/ConfigArray.h"

#include "Config/ConfigSection.h"

void ConfigArray::Add(Config* other)
{
	other->m_Parent = this;
	m_Entries.push_back(other);
}

Config* ConfigArray::Get(int index) const
{
	return m_Entries[index];
}

bool ConfigArray::Deserialize(JingleScript::Lexer* lexer)
{
	using namespace JingleScript;

	if (lexer->GetToken() != Tokens::LeftSquareBracket)
	{
		lexer->Error("Expected '[', got '%s'", lexer->GetTokenValue().c_str());
		return false;
	}
	
	lexer->NextToken();

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

		ConfigSection* cfgSection = new ConfigSection();
		cfgSection->m_Name = "";
		if (!cfgSection->Deserialize(lexer))
		{
			return false;
		}

		Add(cfgSection);
	}
}

void ConfigArray::Serialize(std::ostringstream& output, std::string prefix) const
{
	output << prefix << m_Name << ": " << m_Entries.size() << std::endl;

	for (auto& v : m_Entries)
	{
		if (v == nullptr)
		{
			continue;
		}

		v->Serialize(output, prefix + " ");
	}
}

std::string ConfigArray::ToString() const
{
	std::stringstream ss;
	ss << Super::ToString();

	ss << ", ";
	ss << "Count=" << m_Entries.size();

	int index = 0;
	for (auto entry : m_Entries)
	{
		ss << ", ";
		ss << "Entry_" << index++ << "=" << entry->AsString();
		index++;
	}

	return ss.str();
}
