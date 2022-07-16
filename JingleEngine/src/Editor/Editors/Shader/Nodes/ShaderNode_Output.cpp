#include "ShaderNode_Output.h"

BEGIN_CLASS_LINK(ShaderNode_Output)
	ADD_ATTRIBUTE(InPin);
	LINK_VARIABLE(Diffuse);
	
	ADD_ATTRIBUTE(InPin);
	LINK_VARIABLE(Normal);

	ADD_ATTRIBUTE(InPin);
	LINK_VARIABLE(Metallic);

	ADD_ATTRIBUTE(InPin);
	LINK_VARIABLE(Roughness);

	ADD_ATTRIBUTE(InPin);
	LINK_VARIABLE(Ambient);

	LINK_CONSTRUCTOR();
END_CLASS_LINK()

void ShaderNode_Output::Compile(std::unordered_map<std::string, ShaderNode*> inputs, std::stringstream& output)
{
	ShaderNode* diffuse = inputs["Diffuse"];
	ShaderNode* normal = inputs["Normal"];
	ShaderNode* metallic = inputs["Metallic"];
	ShaderNode* roughness = inputs["Roughness"];
	ShaderNode* ambient = inputs["Ambient"];

	output << fmt::format(
			"PBR({}, {}, {}, {}, {});", 
			diffuse ? diffuse->GetVariableName() : "vec4(0.0)", 
			normal ? normal->GetVariableName() : "vec4(0.0)", 
			metallic ? metallic->GetVariableName() : "vec4(0.0)", 
			roughness ? roughness->GetVariableName() : "vec4(0.0)", 
			ambient ? ambient->GetVariableName() : "vec4(0.0)"
		) << std::endl;
}
