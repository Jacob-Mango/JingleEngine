#include "Editor/Editors/Shader/ShaderNode.h"

BEGIN_CLASS_LINK(ShaderNode)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

ShaderNode::ShaderNode()
{

}

ShaderNode::~ShaderNode()
{

}

void ShaderNode::Compile(std::unordered_map<std::string, ShaderNode*> inputs, std::stringstream& output)
{

}
