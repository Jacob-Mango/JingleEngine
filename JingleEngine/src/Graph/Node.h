#pragma once

#include "Core/Core.h"

#include "Config/ConfigAsset.h"

#include "Graph/Pin.h"

class Graph;
class GraphPanel;

class NodeConnectionArray;

class Node : public JingleScript::Object, public ObjectProperty
{
	DEFINE_CLASS(Node, JingleScript::Object);

	friend GraphPanel;
	friend Graph;

private:
	Graph* m_Graph;
	NodeConnectionArray* m_ConnectionsData;

	float m_EditorPositionX;
	float m_EditorPositionY;

	std::unordered_map<OutPin*, std::pair<Node*, InPin*>> m_Connections;

	std::vector<InPin*> m_InPins;
	std::vector<OutPin*> m_OutPins;

public:
	Node();
	~Node();

	void OnCreate();
	void OnSerialize();

};

class NodeArray : public JingleScript::Array<Node*>
{
	DEFINE_CLASS(NodeArray, JingleScript::Array<Node*>);

public:
	NodeArray() {}
	~NodeArray() {}

};

class NodeConnection : public JingleScript::Object, public ObjectProperty
{
	DEFINE_CLASS(NodeConnection, JingleScript::Object);

public:
	NodeConnection() {}
	~NodeConnection() {}

	std::string Node;
	std::string Pin;

};

class NodeConnectionArray : public JingleScript::Array<NodeConnection*>
{
	DEFINE_CLASS(NodeConnectionArray, JingleScript::Array<NodeConnection*>);

public:
	NodeConnectionArray() {}
	~NodeConnectionArray() {}

};
