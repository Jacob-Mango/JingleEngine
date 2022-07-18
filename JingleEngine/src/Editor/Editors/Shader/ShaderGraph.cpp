#include "Editor/Editors/Shader/ShaderGraph.h"

#include "Editor/Editors/Shader/ShaderEditor.h"
#include "Editor/Editors/Shader/ShaderNode.h"

#include "Editor/Editors/Shader/Nodes/ShaderNode_Output.h"

BEGIN_CLASS_LINK(ShaderGraph)
	LINK_NAMED_VARIABLE(Variables, m_Variables);

	LINK_CONSTRUCTOR();

	LINK_METHOD(OnSerializeVariables);
	LINK_METHOD(OnDeserializeVariables);
	LINK_METHOD(Editor_OnRenderVariables);
END_CLASS_LINK()

ShaderGraph::ShaderGraph()
{

}

ShaderGraph::~ShaderGraph()
{
	
}

JingleScript::Type* ShaderGraph::GetNodeType() const
{
	return ShaderNode::StaticType();
}

bool ShaderGraph::OnInsertNode(Node* node)
{
	if (node->GetType()->IsInherited(ShaderNode_Output::StaticType()))
	{
		if (m_OutputNode)
		{
			return false;
		}

		m_OutputNode = node->As<ShaderNode_Output>();
	}

	return Super::OnInsertNode(node);
}

bool ShaderGraph::OnRemoveNode(Node* node)
{
	if (node->GetType()->IsInherited(ShaderNode_Output::StaticType()))
	{
		return false;
	}

	return Super::OnRemoveNode(node);
}

void ShaderGraph::OnSerializeVariables(Config* cfgRoot)
{
	auto cfg = cfgRoot->CreateArray("Variables");

	for (auto& variable : *m_Variables)
	{
		cfg->Set(variable->Serialize());
	}
}

void ShaderGraph::OnDeserializeVariables(Config* cfgRoot)
{
	auto cfg = cfgRoot->Get("Variables");
	if (!cfg)
	{
		return;
	}

	m_Variables->Clear();

	for (auto& cfgVariable : *cfg)
	{
		ShaderVariable* variable = JingleScript::NewObject<ShaderVariable>(cfgVariable.GetLinkedType());
		if (!variable)
		{
			JS_ERROR("Invalid variable type {}", cfgVariable.GetLinkedType());
			continue;
		}

		variable->Deserialize(&cfgVariable);

		if (variable->GetName().empty())
		{
			variable->SetName(variable->GetType()->Name());
		}

		m_Variables->Insert(variable);
	}
}

void ShaderGraph::Editor_OnRenderVariables()
{
#ifdef JE_EDITOR
	static ShaderVariable* Selected = nullptr;

	EditorUI::Render_CellHeader("Variables", true);

	ImGui::TableNextColumn();

	float width = ImGui::GetColumnWidth() * 0.5f;
	float height = ImGui::GetFrameHeight();

	bool addPressed = ImGui::Button("Add", { width, height });
	ImGui::SameLine();
	bool removePressed = ImGui::Button("Remove", { width, height });
	size_t count = m_Variables->Count();
	size_t removeIndex = count;

	size_t index = 0;
	for (auto& variable : *m_Variables)
	{
		ImGui::PushID(std::to_string(index).c_str());

		std::string variableName = variable->GetName();

		EditorUI::Render_CellHeader(fmt::format("[{}]", index).c_str(), true, true);

		ImGui::TableNextColumn();

		ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_None;

		//! We aren't editting the root config so we can't change it's name
		if (variable->HasBase())
		{
			inputFlags = ImGuiInputTextFlags_ReadOnly;
		}

		//ImGuiInputTextFlags_EnterReturnsTrue
		bool selected = Selected == variable;
		if (EditorUI::Render_SelectableInput(selected, ImGuiSelectableFlags_SpanAllColumns, inputFlags, variableName))
		{
			Selected = variable;

			variable->SetName(variableName);
		}

		variable->Editor_Render();

		ImGui::PopID();

		if (Selected == variable && !variable->HasBase())
		{
			removeIndex = index;
		}

		index++;
	}

	if (addPressed)
	{
		JingleScript::Type* variableType = ShaderVariable::StaticType();
		ShaderVariable* variable = variableType->New<ShaderVariable>();

		m_Variables->Insert(variable);
	}

	if (removePressed)
	{
		// removeIndex
	}
#endif
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

		m_CompiledNodes[node] = compiled;

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
		//! Moves the node to be processed earlier as it is depended on earlier

		compiled = it->second;

		if (m_RootNode)
		{
			m_RootNode->m_Previous = compiled;
			compiled->m_Next = m_RootNode;
		}

		m_RootNode = compiled->m_Root;
		m_RootNode->m_Previous = nullptr;
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

void ShaderGraph::Compile()
{
	if (!m_OutputNode)
	{
		JS_ERROR("No valid output node in the shader graph.");
		return;
	}

	m_RootNode = nullptr;
	AddNode(m_OutputNode);

	std::stringstream stream;

	ShaderCompileNode* node = m_RootNode;
	while (node != nullptr)
	{
		if (!node->m_Node->Compile(node->m_Inputs, stream))
		{
			JS_INFO("Failed to compile shader.");
			break;
		}

		node = node->m_Next;
	}

	JS_INFO("Shader:\n{}", stream.str());
}
