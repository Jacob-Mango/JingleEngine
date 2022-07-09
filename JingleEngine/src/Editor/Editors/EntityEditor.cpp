#include "Editor/Editors/EntityEditor.h"

#include "Core/Application.h"

#include "Editor/EditorModule.h"

BEGIN_CLASS_LINK(EntityEditor);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

EntityEditor::EntityEditor() : Editor()
{
	OpenPanel("EditorViewportPanel");
	OpenPanel("EntityPropertiesPanel");
	OpenPanel("EntityHierarchyPanel");
	OpenPanel("ContentBrowserPanel");
}

EntityEditor::~EntityEditor()
{

}

void EntityEditor::New()
{
	SetAssetID({});

	GetEditorModule()->SetFileName(this);
}

void EntityEditor::Open(AssetID id)
{
	SetAssetID(id);

	GetEditorModule()->SetFileName(this);
}

bool SaveTo(AssetID id, Entity* entity)
{
	ConfigAsset* cfgAsset = AssetModule::Get<ConfigAsset>(id);
	ConfigSection* cfg = cfgAsset->Get();

	cfgAsset->Serialize(entity);

	return cfgAsset->OnSave();
}

void EntityEditor::SaveAs(AssetID id)
{
	Entity* entity = Application::Get()->GetScene();
	
	if (SaveTo(id, entity))
	{
		SetAssetID(id);
	}

	GetEditorModule()->SetFileName(this);
}

void EntityEditor::Save()
{
	Entity* entity = Application::Get()->GetScene();

	SaveTo(GetAssetID(), entity);

	GetEditorModule()->SetFileName(this);
}
