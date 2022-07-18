#include "Editor/Editors/Shader/ShaderEditor.h"

#include "Core/Application.h"

#include "Editor/EditorModule.h"

#include "Editor/Editors/Shader/ShaderNode.h"

#include <filesystem>

using namespace JingleScript;

BEGIN_CLASS_LINK(ShaderEditor);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

ShaderEditor::ShaderEditor() : GraphEditor()
{
	OpenPanel("GraphPanel");
	OpenPanel("GraphPropertiesPanel");
}

ShaderEditor::~ShaderEditor()
{

}

void ShaderEditor::New()
{
	SetAssetID({});

	//m_Graph = NewObject<ShaderGraph>("ShaderGraph");
	//if (!m_Graph)
	//{
	//	return;
	//}
	//
	//m_Graph->m_Editor = this;
	//m_Config = m_Graph->Serialize();

	GetEditorModule()->SetFileName(this);
}

void ShaderEditor::Open(AssetID id)
{
	std::filesystem::path p = id.GetPath();
	if (p.extension() == ".shader")
	{
		id = p.replace_extension().string();
	}

	SetAssetID(id);

	m_Shader = AssetModule::Get<Shader>(GetAssetID());

	GetEditorModule()->SetFileName(this);
}

void ShaderEditor::SaveAs(AssetID id)
{
}

void ShaderEditor::Save()
{
	m_Shader->OnSave();

	GetEditorModule()->SetFileName(this);
}

void ShaderEditor::OnRenderMenu()
{
	Super::OnRenderMenu();

	if (ImGui::BeginMenu("Shader"))
	{
		if (ImGui::MenuItem("Compile", NULL))
		{
			ShaderGraph* graph = m_Shader->GetGraph();
			graph->Compile();
		}

		ImGui::EndMenu();
	}
}

void ShaderEditor::OnSave()
{
	
}
