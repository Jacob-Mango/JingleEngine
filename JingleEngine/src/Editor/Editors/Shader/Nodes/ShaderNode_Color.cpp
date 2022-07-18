#include "ShaderNode_Color.h"

BEGIN_CLASS_LINK(ShaderNode_Color)
	ADD_ATTRIBUTE(OutPin);
	ADD_ATTRIBUTE(Property);
	LINK_VARIABLE(Color);
	
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

bool ShaderNode_Color::Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output)
{
	output << fmt::format(
			"vec4 {} = vec4({}, {}, {}, {});", 
			GetVariableName("Color"),
			Color.x,
			Color.y,
			Color.z,
			Color.w
		) << std::endl;
	
	return true;
}
