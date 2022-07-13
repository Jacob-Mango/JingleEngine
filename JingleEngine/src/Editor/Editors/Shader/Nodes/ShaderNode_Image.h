#pragma once

#include "Editor/Editors/Shader/ShaderNode.h"

class ShaderNode_Image : public ShaderNode
{
	DEFINE_CLASS(ShaderNode_Image, ShaderNode);

public:
	glm::vec2 UV;
	glm::vec4 Output;

public:
	ShaderNode_Image() {}
	
public:
	virtual bool IsValue() const override { return InPinSet("UV"); }
	virtual bool IsVariable() const override { return !InPinSet("UV"); }
	
};
