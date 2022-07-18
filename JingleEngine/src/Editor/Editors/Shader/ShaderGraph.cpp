#include "Editor/Editors/Shader/ShaderGraph.h"

#include "Editor/Editors/Shader/ShaderEditor.h"
#include "Editor/Editors/Shader/ShaderNode.h"

#include "Editor/Editors/Shader/Nodes/ShaderNode_Output.h"

BEGIN_CLASS_LINK(ShaderGraph)
	LINK_CONSTRUCTOR();
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
