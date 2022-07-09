#include "Editor/Editors/EntityEditor.h"

#include "Core/Application.h"

#include "Editor/EditorModule.h"

using namespace JingleScript;

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

	m_Entity = NewObject<Entity>("Entity");
	m_Config = m_Entity->Serialize();

	GetEditorModule()->SetFileName(this);
}

void EntityEditor::Open(AssetID id)
{
	SetAssetID(id);

	ConfigAsset* cfgAsset = AssetModule::Get<ConfigAsset>(GetAssetID());
	m_Config = cfgAsset->Get();

	m_Entity = JingleScript::NewObject<Entity>(m_Config->GetLinkedType());
	if (m_Entity)
	{
		cfgAsset->Deserialize(m_Entity);
	}

	GetEditorModule()->SetFileName(this);
}

void EntityEditor::SaveAs(AssetID id)
{
	ConfigAsset* cfgAsset = AssetModule::Get<ConfigAsset>(id);

	cfgAsset->Serialize(m_Entity);
	m_Config = cfgAsset->Get();

	if (cfgAsset->OnSave())
	{
		SetAssetID(id);
	}

	GetEditorModule()->SetFileName(this);
}

void EntityEditor::Save()
{
	ConfigAsset* cfgAsset = AssetModule::Get<ConfigAsset>(GetAssetID());

	cfgAsset->Serialize(m_Entity);
	m_Config = cfgAsset->Get();

	cfgAsset->OnSave();

	GetEditorModule()->SetFileName(this);
}
