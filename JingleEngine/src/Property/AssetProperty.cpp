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

void AssetProperty::OnRender(void*& data)
{
	std::string path;

	Asset* asset = static_cast<Asset*>(data);
	if (asset)
	{
		path = asset->GetAssetID().GetPath();
	}

	std::string id = PointerToString(data);

	ImGui::PushID(id.c_str());

	Editor::Render_CellHeader(GetPropertyAttribute()->GetName());

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
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::PopID();
}
