#include "Property/AssetProperty.h"

#include "Asset/Asset.h"

using namespace JingleScript;

bool AssetProperty::OnSerialize(Config* cfg, void*& data)
{
	JS_TRACE(Tracers::Property);

	Asset* asset = static_cast<Asset*>(data);
	if (asset == nullptr)
	{
		//! TODO: Remove entry
		return true;
	}

	std::string path = asset->GetAssetID().GetPath();

	cfg->SetValue(path);

	return true;
}

bool AssetProperty::OnDeserialize(Config* cfg, void*& data)
{
	JS_TRACE(Tracers::Property);

	if (!cfg)
	{
		return true;
	}

	AssetID id = *cfg->GetValuePtr();

	Asset* asset = AssetModule::Get<Asset>(id, GetPropertyType());

	data = (void*) asset;

	return true;
}
