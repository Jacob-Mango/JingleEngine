#pragma once

#include "Editor/Editor.h"

class GraphEditor : public Editor
{
	DEFINE_CLASS(GraphEditor, Editor);

public:
	std::vector<Node*> SelectedNodes;
	std::vector<Edge> SelectedEdges;

public:
	GraphEditor();
	~GraphEditor();

};
