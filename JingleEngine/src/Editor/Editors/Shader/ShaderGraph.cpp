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
		if (m_Editor->m_OutputNode)
		{
			return false;
		}

		m_Editor->m_OutputNode = node->As<ShaderNode_Output>();
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
