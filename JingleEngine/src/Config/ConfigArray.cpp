#include "Config/ConfigArray.h"

#include "Config/ConfigAsset.h"
#include "Config/ConfigSection.h"

using namespace JingleScript;

ConfigArray::ConfigArray()
{

}

ConfigArray::~ConfigArray()
{
	for (auto& entry : m_Entries)
	{
		delete entry;
	}
}

void ConfigArray::Add(Config* other)
{
	other->m_Parent = this;
	m_Entries.push_back(other);
}

Config* ConfigArray::Get(int index) const
{
	return m_Entries[index];
}

bool ConfigArray::Deserialize(Lexer* lexer)
{
	if (lexer->GetToken() != Tokens::LeftSquareBracket)
	{
		lexer->Error("Expected '[', got '%s'", lexer->GetTokenValue().c_str());
		return false;
	}
	
	lexer->NextToken();

	while (lexer->HasNext())
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

		std::pair<std::string, std::string> typeAndName;
		if (!DeserializeTypeAndName(lexer, typeAndName))
		{
			return false;
		}

		if (lexer->GetToken() != Tokens::LeftCurlyBracket)
		{
			lexer->Error("Expected '{', got '%s'", lexer->GetTokenValue().c_str());
			return false;
		}

		ConfigSection* cfgSection = new ConfigSection();
		cfgSection->m_CType = typeAndName.first;
		cfgSection->m_Name = typeAndName.second;

		Add(cfgSection);

		if (!cfgSection->Deserialize(lexer))
		{
			return false;
		}
	}

	lexer->Error("Unexpected end of file.");
	return false;
}

bool ConfigArray::Serialize(std::stringstream& output, std::string prefix) const
{
	bool canOutput = false;
	std::stringstream tempOut;

	tempOut << prefix << SerializeTypeAndName() << "[" << std::endl;

	int index = 0;
	for (auto& v : m_Entries)
	{
		index++;

		if (v == nullptr)
		{
			continue;
		}

		canOutput |= v->Serialize(tempOut, prefix + " ");

		if (index < m_Entries.size())
		{
			tempOut << ", " << std::endl;
		}
	}

	tempOut << std::endl << prefix << "]";

	if (canOutput)
	{
		output << tempOut.str();
	}

	return canOutput;
}
