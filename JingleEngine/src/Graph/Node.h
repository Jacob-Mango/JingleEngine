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
	Graph* m_Graph = nullptr;
	NodeConnectionArray* m_ConnectionsData = nullptr;

	float m_EditorPositionX;
	float m_EditorPositionY;

	std::unordered_map<OutPin*, std::pair<Node*, InPin*>> m_OutConnections;
	std::unordered_map<InPin*, std::pair<Node*, OutPin*>> m_InConnections;

	std::unordered_map<std::string, InPin*> m_InPins;
	std::unordered_map<std::string, OutPin*> m_OutPins;

	std::vector<InPin*> m_temp_InPins;
	std::vector<OutPin*> m_temp_OutPins;

public:
	Node();
	~Node();

	void OnCreate();
	void OnSerialize();

	void CreateConnection(OutPin* out, std::pair<Node*, InPin*> in);
	void DeleteConnection(OutPin* out);

	bool InPinSet(const char* name) const;
	bool OutPinSet(const char* name) const;

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
