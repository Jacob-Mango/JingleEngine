#include "Asset.h"

#include "AssetManager.h"

#include <string>

Asset::Asset(const GUID &guid) : m_GUID(guid)
{
}

Asset::~Asset()
{
	AssetManager::Unload(GetGUID());
}

std::string Asset::GetPath()
{
	return m_GUID.GetPath();
}

std::string Asset::GetExtension()
{
	std::string path = GetPath();
	return path.substr(path.find_last_of(".") + 1, 16);
}

GUID Asset::GetGUID()
{
	return m_GUID;
}

bool Asset::IsLoaded()
{
	return m_IsLoaded;
}

bool Asset::OnLoad()
{
	return false;
}

std::string Asset::ToString()
{
	std::stringstream ss;

	ss << base::ToString();
	ss << ", ";

	ss << "Path=" << GetPath();

	return ss.str();
}
