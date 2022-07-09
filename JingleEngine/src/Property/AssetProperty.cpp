#include "Property/AssetProperty.h"

#include "Asset/Asset.h"

#include "Config/Config.h"
#include "Config/ConfigAsset.h"
#include "Config/ConfigSection.h"
#include "Config/ConfigValue.h"

using namespace JingleScript;

bool AssetProperty::OnSerialize(Config* cfgRoot, void*& data)
{
	JS_TRACE(Tracers::Property);

	Asset* asset = static_cast<Asset*>(data);
	if (asset == nullptr)
	{
		//! TODO: Remove entry
		return true;
	}

	std::string value = asset->GetAssetID().GetPath();

	//! TODO: compare with default and return 'false' if it is default

	cfgRoot->CreateValue(GetPropertyAttribute()->GetName(), value);

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

void AssetProperty::Editor_OnRender(void*& data)
{
	std::string path;

	Asset* asset = static_cast<Asset*>(data);
	if (asset)
	{
		path = asset->GetAssetID().GetPath();
	}

	std::string name = GetPropertyAttribute()->GetName();
	EditorUI::Render_CellHeader(name);

	ImGui::TableNextColumn();

	if (path.empty())
	{
		ImGui::TextUnformatted("None...");
	}
	else
	{
		ImGui::TextUnformatted(path.c_str());
	}

	if (ImGui::BeginDragDropTarget())
	{
		auto payload = ImGui::AcceptDragDropPayload("content_browser");
		if (payload)
		{
			AssetID id = *((AssetID*)payload->Data);
			Asset* asset = AssetModule::Get<Asset>(id, GetPropertyType());

			data = (void*)asset;

			//JingleScript::Function<void, std::string> Script_Editor_OnPropertyChanged = { "Editor_OnPropertyChanged", GetPropertyOwner()->GetPropertyType() };
			//Script_Editor_OnPropertyChanged[GetPropertyOwner()](name);
			GetPropertyOwner()->Editor_OnPropertyChanged(name);
		}

		ImGui::EndDragDropTarget();
	}
}
