#pragma once

#include "Core/Core.h"

#include "Config/ConfigAsset.h"

#include "Graph/Pin.h"
#include "Graph/Node.h"

class Graph : public JingleScript::Object, public ObjectProperty
{
	DEFINE_CLASS(Graph, JingleScript::Object);

private:
	NodeArray* m_Nodes;

public:
	Graph();
	~Graph();

public:
	void OnSerializeNodes(Config* cfg);
	void OnDeserializeNodes(Config* cfg);
	void Editor_OnRenderNodes();

};