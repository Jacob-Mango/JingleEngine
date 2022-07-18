#pragma once

#include "Graph/Graph.h"

class ShaderEditor;

class ShaderNode_Output;

class ShaderGraph : public Graph
{
	DEFINE_CLASS(ShaderGraph, Graph);

	friend ShaderEditor;

private:
	ShaderNode_Output* m_OutputNode = nullptr;

public:
	ShaderGraph();
	~ShaderGraph();

public:
	virtual JingleScript::Type* GetNodeType() const override;

	virtual bool OnInsertNode(Node* node) override;
	virtual bool OnRemoveNode(Node* node) override;

public:
	void Compile();

};
