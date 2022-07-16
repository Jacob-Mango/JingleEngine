#include "ShaderNode_Image.h"

BEGIN_CLASS_LINK(ShaderNode_Image)
	ADD_ATTRIBUTE(InPin);
	LINK_VARIABLE(UV);
	
	ADD_ATTRIBUTE(OutPin);
	LINK_VARIABLE(Output);

	LINK_CONSTRUCTOR();
END_CLASS_LINK()

void ShaderNode_Image::Compile(std::unordered_map<std::string, ShaderNode*> inputs, std::stringstream& output)
{

}
