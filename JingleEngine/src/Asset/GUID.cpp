#include "GUID.h"

#include "AssetModule.h"

GUID::GUID()
{

}

GUID::GUID(GUIDv id) : m_Value(id)
{

}

GUID::GUID(std::string path) : m_Path(path)
{
	m_Value = AssetModule::ConvertPath(m_Path);
}

GUID::GUID(const GUID& other)
{
	m_Value = other.m_Value;
	m_Path = other.m_Path;
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
