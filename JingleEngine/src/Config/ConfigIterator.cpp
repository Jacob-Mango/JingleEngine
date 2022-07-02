#include "Config/ConfigIterator.h"

#include "Config/Config.h"
#include "Config/ConfigSection.h"
#include "Config/ConfigValue.h"

ConfigIterator::ConfigIterator()
{
	m_Config = nullptr;
}

ConfigIterator::ConfigIterator(Config* cfg)
{
	m_Config = cfg;

	std::unordered_set<std::string> names;

	ConfigSection* section = dynamic_cast<ConfigSection*>(m_Config);
	while (section != nullptr)
	{
		for (auto& [name, cfg] : section->m_Entries)
		{
			names.insert(name);
		}

		section = section->GetBase();
	}

	for (auto& name : names)
	{
		m_Names.push(name);
	}
}

ConfigIterator::reference ConfigIterator::operator*() const
{
	Config* cfg = nullptr;
	if (!m_Names.empty())
	{
		cfg = m_Config->Get(m_Names.front());;
	}

	return *cfg;
}

ConfigIterator::pointer ConfigIterator::operator->()
{
	Config* cfg = nullptr;
	if (!m_Names.empty())
	{
		cfg = m_Config->Get(m_Names.front());;
	}

	return cfg;
}

ConfigIterator& ConfigIterator::operator++()
{
	if (!m_Names.empty())
	{
		m_Names.pop();
	}

	if (m_Names.empty())
	{
		m_Config = nullptr;
	}

	return *this;
}

ConfigIterator ConfigIterator::operator++(int)
{
	ConfigIterator it = *this;
	++it;
	return it;
}

bool operator==(const ConfigIterator& a, const ConfigIterator& b)
{
	return a.m_Config == b.m_Config;
}

bool operator!=(const ConfigIterator& a, const ConfigIterator& b)
{
	return a.m_Config != b.m_Config;
}
