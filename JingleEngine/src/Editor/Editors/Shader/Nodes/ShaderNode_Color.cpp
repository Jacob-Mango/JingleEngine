#include "ShaderNode_Color.h"

BEGIN_CLASS_LINK(ShaderNode_Color)
	ADD_ATTRIBUTE(OutPin);
	LINK_VARIABLE(Output);
	
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

void ShaderNode_Color::Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output)
{

}
