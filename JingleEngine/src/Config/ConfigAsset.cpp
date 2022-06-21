#include "Config/ConfigAsset.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(ConfigAsset)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

ConfigAsset::ConfigAsset()
{
	m_Properties = new PropertyObject();
}

ConfigAsset::~ConfigAsset()
{
	delete m_Properties;
}

bool ConfigAsset::OnLoad()
{
	std::string path = GetPath();
	if (path.empty())
	{
		return false;
	}

	Ref<Lexer> lexer = Lexer::ParseFile(path);
	if (!lexer->HasNext())
	{
		return false;
	}

	if (!Deserialize(lexer))
	{
		return false;
	}

	if (!m_Properties->OnDeserialize(this))
	{
		return false;
	}

	return true;
}

bool ConfigAsset::WriteToObject(JingleScript::Object* instance)
{
	m_Properties->WriteObject(instance);
	return true;
}

bool ConfigAsset::ReadFromObject(JingleScript::Object* instance)
{
	m_Properties->ReadObject(instance);
	return true;
}
