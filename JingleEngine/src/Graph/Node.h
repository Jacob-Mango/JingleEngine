#pragma once

#include "Core/Core.h"

#include "Config/ConfigAsset.h"

#include "Graph/Pin.h"

class Graph;

class Node : public JingleScript::Object, public ObjectProperty
{
	DEFINE_CLASS(Node, JingleScript::Object);

	friend Graph;
	friend Pin;

private:
	Graph* m_Graph;

public:
	Node();
	~Node();

	void OnCreate();

};

class NodeArray : public JingleScript::Array<Node*>
{
	DEFINE_CLASS(NodeArray, JingleScript::Array<Node*>);

public:
	NodeArray() {}
	~NodeArray() {}

};
