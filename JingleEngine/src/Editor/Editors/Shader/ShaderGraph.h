#pragma once

#include "Graph/Graph.h"

class ShaderGraph : public Graph
{
	DEFINE_CLASS(ShaderGraph, Graph);

public:
	ShaderGraph();
	~ShaderGraph();

public:
	virtual JingleScript::Type* GetNodeType() const override;

};
