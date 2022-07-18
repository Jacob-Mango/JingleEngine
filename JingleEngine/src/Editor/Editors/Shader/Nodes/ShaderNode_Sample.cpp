#include "ShaderNode_Sample.h"

BEGIN_CLASS_LINK(ShaderNode_Sample)
	ADD_ATTRIBUTE(InPin);
	LINK_VARIABLE(Image);

	ADD_ATTRIBUTE(InPin);
	LINK_VARIABLE(UV);
	
	ADD_ATTRIBUTE(OutPin);
	LINK_VARIABLE(Output);

	LINK_CONSTRUCTOR();
END_CLASS_LINK()

void ShaderNode_Sample::Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output)
{

}
