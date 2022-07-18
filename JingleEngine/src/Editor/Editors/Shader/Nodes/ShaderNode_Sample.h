#pragma once

#include "Editor/Editors/Shader/ShaderNode.h"

#include "Rendering/Image.h"

class ShaderNode_Sample : public ShaderNode
{
	DEFINE_CLASS(ShaderNode_Sample, ShaderNode);

public:
	glm::vec2 UV;
	Image* Image;

	glm::vec4 Output;

public:
	ShaderNode_Sample() {}
	
public:
	virtual bool IsValue() const override { return true; }

	virtual void Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output) override;
	
};
