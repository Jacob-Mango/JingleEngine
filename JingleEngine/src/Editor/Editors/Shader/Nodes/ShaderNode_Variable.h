#pragma once

#include "Editor/Editors/Shader/ShaderNode.h"

class ShaderNode_Variable : public ShaderNode
{
	DEFINE_CLASS(ShaderNode_Variable, ShaderNode);

	std::string VariableName;
	Type* VariableType;

public:
	ShaderNode_Variable() {}
	
public:
	virtual bool IsVariable() const override { return true; }

	virtual std::string& GetVariableName() const override { return VariableName; }

	virtual void Compile(std::unordered_map<std::string, ShaderNode*> inputs, std::stringstream& output) override;
	
};
