#pragma once

#include "Editor/Editors/Shader/ShaderNode.h"

class ShaderNode_Variable : public ShaderNode
{
	DEFINE_CLASS(ShaderNode_Variable, ShaderNode);

	std::string Name;
	JingleScript::Type* Output;

public:
	ShaderNode_Variable() {}
	
public:
	virtual bool IsVariable() const override { return true; }

	virtual const std::string& GetVariableName(const std::string& pin) const override { return Name; }

	virtual void Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output) override;
	
	void OnSerializeVariableType(Config* cfg);
	void OnDeserializeVariableType(Config* cfg);
	void Editor_OnRenderVariableType();

};
