#pragma once

#include "Editor/Editors/Shader/ShaderNode.h"

class ShaderNode_Output : public ShaderNode
{
	DEFINE_CLASS(ShaderNode_Output, ShaderNode);

public:
	glm::vec4 Diffuse;
	glm::vec4 Normal;
	glm::vec4 Metallic;
	glm::vec4 Roughness;
	glm::vec4 Ambient;

public:
	ShaderNode_Output() {}
	
public:
	virtual bool IsFunction() const override { return true; }

	virtual bool Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output) override;
	
};
