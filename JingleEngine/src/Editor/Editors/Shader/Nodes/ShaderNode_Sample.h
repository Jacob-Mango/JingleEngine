#pragma once

#include "Editor/Editors/Shader/ShaderNode.h"

class ShaderNode_Sample : public ShaderNode
{
	DEFINE_CLASS(ShaderNode_Sample, ShaderNode);

public:
	glm::vec2 UV;
	glm::vec4 Output;

public:
	ShaderNode_Sample() {}
	
public:
	virtual bool IsValue() const override { return true; }

	virtual void Compile(std::unordered_map<std::string, ShaderNode*> inputs, std::stringstream& output) override;
	
};
