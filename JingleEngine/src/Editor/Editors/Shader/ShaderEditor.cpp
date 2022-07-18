#include "Editor/Editors/Shader/ShaderEditor.h"

#include "Core/Application.h"

#include "Editor/EditorModule.h"

#include "Editor/Editors/Shader/ShaderNode.h"

#include "Editor/Editors/Shader/Nodes/ShaderNode_Output.h"

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

	m_Graph = NewObject<ShaderGraph>("ShaderGraph");
	if (!m_Graph)
	{
		return;
	}

	m_Graph->m_Editor = this;
	m_Config = m_Graph->Serialize();

	GetEditorModule()->SetFileName(this);
}

void ShaderEditor::Open(AssetID id)
{
	SetAssetID(id);

	ConfigAsset* cfgAsset = AssetModule::Get<ConfigAsset>(GetAssetID());

	m_OutputNode = nullptr;
	m_Config = cfgAsset->Get();

	m_Graph = JingleScript::NewObject<ShaderGraph>(m_Config->GetLinkedType());
	if (!m_Graph)
	{
		return;
	}

	m_Graph->m_Editor = this;
	cfgAsset->Deserialize(m_Graph);

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

void ShaderEditor::OnRenderMenu()
{
	Super::OnRenderMenu();

	if (ImGui::BeginMenu("Shader"))
	{
		if (ImGui::MenuItem("Compile", NULL))
		{
			Compile();
		}

		ImGui::EndMenu();
	}
}

//! NOTE: Portions of this will be moved to the actual shader asset. 
//! 
//! BUGS: The program will crash if there is a node recurssion. This will be fixed by preventing recurssive nodes to be added once a callback for 'CreateConnection' is made.

static ShaderCompileNode* m_RootNode;

static std::unordered_map<ShaderNode*, ShaderCompileNode*> m_CompiledNodes;

struct ShaderCompileNode
{
	ShaderCompileNode* m_Previous = nullptr;
	ShaderCompileNode* m_Next = nullptr;

	ShaderCompileNode* m_Root = nullptr;

	ShaderNode* m_Node;
	std::unordered_map<std::string, std::pair<ShaderNode*, std::string>> m_Inputs;
};

static ShaderCompileNode* AddNode(ShaderNode* node)
{
	ShaderCompileNode* compiled = nullptr;

	auto it = m_CompiledNodes.find(node);
	if (it == m_CompiledNodes.end())
	{
		compiled = new ShaderCompileNode();
		compiled->m_Node = node;

		if (m_RootNode == nullptr)
		{
			m_RootNode = compiled;
		}
		else if (m_RootNode != compiled)
		{
			compiled->m_Next = m_RootNode;
			m_RootNode->m_Previous = compiled;
			m_RootNode = compiled;
		}
	}
	else
	{
		compiled = it->second;

		if (m_RootNode)
		{
			m_RootNode->m_Previous = compiled;
			compiled->m_Next = m_RootNode;
		}

		m_RootNode = compiled->m_Root;
	}

	auto& inputs = node->GetInConnections();

	ShaderCompileNode* root = compiled;

	for (auto& input : inputs)
	{
		std::string inputName = input.first->GetName();

		ShaderNode* inputNode = input.second.first->As<ShaderNode>();
		std::string outputName = input.second.second->GetName();

		compiled->m_Inputs[inputName] = { inputNode, outputName };

		root = AddNode(inputNode);
	}

	compiled->m_Root = root;

	return root;
}

void ShaderEditor::Compile()
{
	if (!m_OutputNode)
	{
		JS_ERROR("No valid output node in the shader graph.");
		return;
	}

	JS_INFO("Compiling with {}", m_OutputNode->GetName());

	m_RootNode = nullptr;
	AddNode(m_OutputNode);

	ShaderCompileNode* node = m_RootNode;
	while (node != nullptr)
	{
		JS_INFO("Node {}", node->m_Node->GetName());

		node = node->m_Next;
	}
}

void ShaderEditor::OnSave()
{
	
}
