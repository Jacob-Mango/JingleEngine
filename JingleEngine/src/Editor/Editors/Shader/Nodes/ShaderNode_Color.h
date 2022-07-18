#pragma once

#include "Editor/Editors/Shader/ShaderNode.h"

class ShaderNode_Color : public ShaderNode
{
	DEFINE_CLASS(ShaderNode_Color, ShaderNode);
	
public:
	glm::vec4 Output;

public:
	ShaderNode_Color() {}
	
public:
	virtual bool IsVariable() const override { return true; }

	virtual void Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output) override;
	
};
