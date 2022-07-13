#pragma once

#include "Editor/Editors/Shader/ShaderNode.h"

class ShaderNode_Variable : public ShaderNode
{
	DEFINE_CLASS(ShaderNode_Variable, ShaderNode);

public:
	ShaderNode_Variable() {}
	
public:
	virtual bool IsVariable() const override { return true; }
	
};
