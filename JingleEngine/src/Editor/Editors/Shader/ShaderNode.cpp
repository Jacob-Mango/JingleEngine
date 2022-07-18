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

bool ShaderNode::Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output)
{
	return false;
}
