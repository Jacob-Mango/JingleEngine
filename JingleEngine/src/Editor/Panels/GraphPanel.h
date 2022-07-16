#pragma once

#include "Editor/EditorPanel.h"

#include "Graph/Node.h"

typedef std::pair<std::pair<Node*, OutPin*>, std::pair<Node*, InPin*>> Edge;

class GraphPanel : public EditorPanel
{
	DEFINE_CLASS(GraphPanel, EditorPanel);

	class Viewport* m_Viewport;

	std::vector<Node*> m_SelectedNodes;
	std::vector<Edge> m_SelectedEdges;

public:
	GraphPanel();

	virtual void OnBeginRender(double DeltaTime) override;
	virtual void OnRender(double DeltaTime) override;
	virtual void OnEndRender(double DeltaTime) override;

};
