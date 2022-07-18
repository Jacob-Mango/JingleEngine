#pragma once

#include "Graph/Graph.h"

class ShaderEditor;

class ShaderGraph : public Graph
{
	DEFINE_CLASS(ShaderGraph, Graph);

	friend ShaderEditor;

private:
	ShaderEditor* m_Editor = nullptr;

public:
	ShaderGraph();
	~ShaderGraph();

public:
	virtual JingleScript::Type* GetNodeType() const override;

	virtual bool OnInsertNode(Node* node) override;
	virtual bool OnRemoveNode(Node* node) override;

};
