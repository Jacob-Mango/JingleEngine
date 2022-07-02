#include "Config/ConfigIterator.h"

#include "Config/Config.h"
#include "Config/ConfigSection.h"
#include "Config/ConfigValue.h"

ConfigIterator::ConfigIterator()
{
}

ConfigIterator::ConfigIterator(Config* cfg)
{
	ConfigSection* section = static_cast<ConfigSection*>(cfg);
	ConfigSection* base = nullptr;
	while (section != nullptr)
	{
		base = section->GetBase();
		if (!base)
		{
			m_IT = section->m_Entries.begin();
		}

		m_Sections.push(section);
		section = base;
	}
}

ConfigIterator::ConfigIterator(Config* cfg, std::nullptr_t)
{
	m_IT = static_cast<ConfigSection*>(cfg)->m_Entries.end();
}

ConfigIterator::reference ConfigIterator::operator*() const
{
	return *(m_IT.operator*().second);
}

ConfigIterator::pointer ConfigIterator::operator->()
{
	return (m_IT.operator*().second);
}

ConfigIterator& ConfigIterator::operator++()
{
	if (!m_Sections.empty())
	{
		if (++m_IT == m_Sections.front()->m_Entries.end())
		{
			m_Sections.pop();

			return ++(*this);
		}
	}
	
	return *this;
}

bool operator==(const ConfigIterator& a, const ConfigIterator& b)
{
	return a.m_IT == b.m_IT && a.m_Sections == b.m_Sections;
}

bool operator!=(const ConfigIterator& a, const ConfigIterator& b)
{
	return a.m_Sections != b.m_Sections || a.m_IT != b.m_IT;
}
