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

bool ShaderNode_Sample::Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output)
{
	auto getVariable = [&](std::string name, std::string def)
		{
			auto it = inputs.find(name);
			if (it == inputs.end())
			{
				return def;
			}

			ShaderNode* node = it->second.first;
			std::string pin = it->second.second;
			if (!node)
			{
				return def;
			}

			return node->GetVariableName(pin);
		};

	std::string varImage = getVariable("Image", "failed");
	std::string varUV = getVariable("UV", "uv");

	output << fmt::format(
			"vec4 {} = texture({}, {});", 
			GetVariableName("Output"),
			varImage,
			varUV
		) << std::endl;

	return true;
}
