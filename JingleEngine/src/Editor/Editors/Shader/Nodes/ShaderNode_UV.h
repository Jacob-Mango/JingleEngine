#pragma once

#include "Editor/Editors/Shader/ShaderNode.h"

class ShaderNode_UV : public ShaderNode
{
	DEFINE_CLASS(ShaderNode_UV, ShaderNode);

public:
	glm::vec2 UV;

public:
	ShaderNode_UV() {}
	
public:
	virtual bool IsVariable() const override { return true; }

	virtual const std::string& GetVariableName() const override { return "uv"; }

	virtual void Compile(std::unordered_map<std::string, ShaderNode*> inputs, std::stringstream& output) override;
	
};
