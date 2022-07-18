#pragma once

#include "Graph/Graph.h"

#include "Editor/Editors/Shader/ShaderVariable.h"

class ShaderEditor;

class ShaderNode_Output;

class ShaderGraph : public Graph
{
	DEFINE_CLASS(ShaderGraph, Graph);

	friend ShaderEditor;

private:
	ShaderNode_Output* m_OutputNode = nullptr;
	ShaderVariableArray* m_Variables = nullptr;

public:
	ShaderGraph();
	~ShaderGraph();

public:
	virtual JingleScript::Type* GetNodeType() const override;

	virtual bool OnInsertNode(Node* node) override;
	virtual bool OnRemoveNode(Node* node) override;

public:
	void OnSerializeVariables(Config* cfg);
	void OnDeserializeVariables(Config* cfg);
	void Editor_OnRenderVariables();

public:
	void Compile();

};
