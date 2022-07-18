#include "ShaderNode_UV.h"

BEGIN_CLASS_LINK(ShaderNode_UV)
	ADD_ATTRIBUTE(OutPin);
	LINK_VARIABLE(UV);
	
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

void ShaderNode_UV::Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output)
{

}
