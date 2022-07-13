#pragma once

#include "Graph/Node.h"

class ShaderNode : public Node
{
	DEFINE_CLASS(ShaderNode, Node);

public:
	ShaderNode();
	~ShaderNode();

	virtual bool IsValue() const { return false; }
	virtual bool IsVariable() const { return false; }
	virtual bool IsFunction() const { return false; }
	virtual bool IsBlock() const { return false; }

};
