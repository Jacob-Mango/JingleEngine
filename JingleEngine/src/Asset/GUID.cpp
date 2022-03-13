#include "GUID.h"

#include "AssetModule.h"

GUID::GUID(GUIDv id) : m_Value(id)
{

}

GUID::GUID(std::string path) : m_Path(path)
{
	m_Value = AssetModule::ConvertPath(m_Path);
}

std::string GUID::GetPath() const
{
	return m_Path;
}

GUIDv GUID::GetValue() const
{
	return m_Value;
}

GUID::operator GUIDv() const
{
	return m_Value;
}
