#include "Graph/Graph.h"

BEGIN_CLASS_LINK(Graph)
	LINK_NAMED_VARIABLE(Nodes, m_Nodes);
	LINK_CONSTRUCTOR();
	LINK_METHOD(OnSerializeNodes);
	LINK_METHOD(OnDeserializeNodes);
	LINK_METHOD(Editor_OnRenderNodes);
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
		node->OnSerialize();
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

	m_Nodes->Clear();

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

		m_Nodes->Insert(node);
	}

	for (auto& node : *m_Nodes)
	{
		node->OnCreate();
	}
}

void Graph::Editor_OnRenderNodes()
{

}
