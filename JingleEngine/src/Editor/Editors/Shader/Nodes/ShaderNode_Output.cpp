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

void ShaderNode_Output::Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output)
{
	auto& diffuse = inputs["Diffuse"];
	auto& normal = inputs["Normal"];
	auto& metallic = inputs["Metallic"];
	auto& roughness = inputs["Roughness"];
	auto& ambient = inputs["Ambient"];

	auto getVariable = [](std::pair<ShaderNode*, std::string>& p, std::string def)
		{
			return p.first ? p.first->GetVariableName(p.second) : def;
		};

	output << fmt::format(
			"PBR({}, {}, {}, {}, {});", 
			getVariable(diffuse, "vec4(0.0)"), 
			getVariable(normal, "vec4(0.0)"), 
			getVariable(metallic, "vec4(0.0)"), 
			getVariable(roughness, "vec4(0.0)"), 
			getVariable(ambient, "vec4(0.0)")
		) << std::endl;
}
