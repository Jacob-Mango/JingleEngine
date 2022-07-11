#include "Editor/Editors/Shader/ShaderEditor.h"

#include "Core/Application.h"

#include "Editor/EditorModule.h"

using namespace JingleScript;

BEGIN_CLASS_LINK(ShaderEditor);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

ShaderEditor::ShaderEditor() : Editor()
{
	OpenPanel("GraphPanel");
	//OpenPanel("GraphVariablesPanel");
}

ShaderEditor::~ShaderEditor()
{

}

void ShaderEditor::New()
{
	SetAssetID({});

	m_Graph = NewObject<ShaderGraph>("ShaderGraph");
	m_Config = m_Graph->Serialize();

	GetEditorModule()->SetFileName(this);
}

void ShaderEditor::Open(AssetID id)
{
	SetAssetID(id);

	ConfigAsset* cfgAsset = AssetModule::Get<ConfigAsset>(GetAssetID());
	m_Config = cfgAsset->Get();

	m_Graph = JingleScript::NewObject<ShaderGraph>(m_Config->GetLinkedType());
	if (m_Graph)
	{
		cfgAsset->Deserialize(m_Graph);
	}

	GetEditorModule()->SetFileName(this);
}

void ShaderEditor::SaveAs(AssetID id)
{
	ConfigAsset* cfgAsset = AssetModule::Get<ConfigAsset>(id);

	cfgAsset->Serialize(m_Graph);
	m_Config = cfgAsset->Get();

	if (cfgAsset->OnSave())
	{
		SetAssetID(id);

		OnSave();
	}

	GetEditorModule()->SetFileName(this);
}

void ShaderEditor::Save()
{
	ConfigAsset* cfgAsset = AssetModule::Get<ConfigAsset>(GetAssetID());

	cfgAsset->Serialize(m_Graph);
	m_Config = cfgAsset->Get();

	if (cfgAsset->OnSave())
	{
		OnSave();
	}

	GetEditorModule()->SetFileName(this);
}

void ShaderEditor::OnSave()
{
	
}
