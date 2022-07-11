#include "Editor/Editors/Shader/ShaderGraph.h"

#include "Editor/Editors/Shader/ShaderNode.h"

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
