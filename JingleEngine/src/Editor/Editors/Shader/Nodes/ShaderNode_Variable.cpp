#include "ShaderNode_Variable.h"

BEGIN_CLASS_LINK(ShaderNode_Variable)
	ADD_ATTRIBUTE(Property);
	LINK_VARIABLE(VariableName);

	ADD_ATTRIBUTE(Property);
	LINK_VARIABLE(VariableType);

	LINK_CONSTRUCTOR();
END_CLASS_LINK()

void ShaderNode_Variable::Compile(std::unordered_map<std::string, ShaderNode*> inputs, std::stringstream& output)
{

}
