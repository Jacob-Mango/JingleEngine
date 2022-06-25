#include "Asset.h"

#include "AssetModule.h"

#include <string>

Asset::Asset()
{
}

Asset::~Asset()
{
	AssetModule::Unload(GetAssetID());
}

BEGIN_CLASS_LINK(Asset)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

std::string Asset::GetPath() const
{
	return m_AssetID.GetPath();
}

std::string Asset::GetExtension() const
{
	std::string path = GetPath();
	return path.substr(path.find_last_of(".") + 1, 16);
}

AssetID Asset::GetAssetID() const
{
	return m_AssetID;
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
