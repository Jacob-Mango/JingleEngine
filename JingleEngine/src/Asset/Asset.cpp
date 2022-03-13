#include "Asset.h"

#include "AssetModule.h"

#include <string>

Asset::Asset(const GUID &guid) : m_GUID(guid)
{
}

Asset::~Asset()
{
	AssetModule::Unload(GetGUID());
}

std::string Asset::GetPath() const
{
	return m_GUID.GetPath();
}

std::string Asset::GetExtension() const
{
	std::string path = GetPath();
	return path.substr(path.find_last_of(".") + 1, 16);
}

GUID Asset::GetGUID() const
{
	return m_GUID;
}

bool Asset::IsLoaded() const
{
	return m_IsLoaded;
}

bool Asset::OnLoad()
{
	return false;
}

std::string Asset::ToString() const
{
	std::stringstream ss;

	ss << Super::ToString();
	ss << ", ";

	ss << "Path=" << GetPath();

	return ss.str();
}
