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

	virtual std::string GetVariableName(const std::string& pin) const override { return "uv"; }

	virtual bool Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output) override;
	
};
