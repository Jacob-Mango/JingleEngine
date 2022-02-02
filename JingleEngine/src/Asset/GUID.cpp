#include "GUID.h"

#include "AssetManager.h"

GUID::GUID(GUIDv id) : m_Value(id)
{

}

GUID::GUID(std::string path) : m_Path(path)
{
	m_Value = AssetManager::ConvertPath(m_Path);
}

std::string GUID::GetPath()
{
	return m_Path;
}

GUIDv GUID::GetValue()
{
	return m_Value;
}

GUID::operator GUIDv()
{
	return m_Value;
}
