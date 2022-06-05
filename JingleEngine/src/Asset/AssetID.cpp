#include "AssetID.h"

#include "AssetModule.h"

AssetID::AssetID()
{

}

AssetID::AssetID(AssetIDv id) : m_Value(id)
{

}

AssetID::AssetID(std::string path) : m_Path(path)
{
	m_Value = AssetModule::ConvertPath(m_Path);
}

AssetID::AssetID(const AssetID& other)
{
	m_Value = other.m_Value;
	m_Path = other.m_Path;
}

std::string AssetID::GetPath() const
{
	return m_Path;
}

AssetIDv AssetID::GetValue() const
{
	return m_Value;
}

AssetID::operator AssetIDv() const
{
	return m_Value;
}
