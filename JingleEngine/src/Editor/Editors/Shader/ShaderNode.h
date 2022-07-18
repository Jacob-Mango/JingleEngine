#pragma once

#include "Graph/Node.h"

class ShaderNode : public Node
{
	DEFINE_CLASS(ShaderNode, Node);

	friend struct ShaderCompileNode* AddNode(ShaderNode* node);

public:
	ShaderNode();
	~ShaderNode();

	virtual bool IsValue() const { return false; }
	virtual bool IsVariable() const { return false; }
	virtual bool IsFunction() const { return false; }
	virtual bool IsBlock() const { return false; }

	virtual std::string GetVariableName(const std::string& pin) const { return GetName(); }

	// When processed as variable, inputs is empty
	virtual bool Compile(std::unordered_map<std::string, std::pair<ShaderNode*, std::string>>& inputs, std::stringstream& output);

};
