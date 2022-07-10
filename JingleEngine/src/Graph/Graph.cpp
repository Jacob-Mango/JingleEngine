#include "Graph/Graph.h"

BEGIN_CLASS_LINK(Graph)
	LINK_NAMED_VARIABLE(Node, m_Nodes);
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::OnSerializeNodes(Config* cfgRoot)
{
	auto cfg = cfgRoot->CreateArray("Nodes");

	for (auto& node : *m_Nodes)
	{
		cfg->Set(node->Serialize());
	}
}

void Graph::OnDeserializeNodes(Config* cfgRoot)
{
	auto cfg = cfgRoot->Get("Nodes");
	if (!cfg)
	{
		return;
	}

	for (auto& cfgNode : *cfg)
	{
		Node* node = JingleScript::NewObject<Node>(cfgNode.GetLinkedType());
		if (!node)
		{
			JS_ERROR("Invalid node type {}", cfgNode.GetLinkedType());
			continue;
		}

		node->m_Graph = this;
		node->Deserialize(&cfgNode);

		if (node->GetName().empty())
		{
			node->SetName(node->GetType()->Name());
		}

		node->OnCreate();

		m_Nodes->Insert(node);
	}
}

void Graph::Editor_OnRenderNodes()
{

}
