#pragma once

#include "Core/Core.h"

#include "Config/ConfigAsset.h"

#include "Graph/Pin.h"

class Graph;
class GraphPanel;

class NodeOutConnectionArray;
class NodeInConnectionArray;

class Node : public JingleScript::Object, public ObjectProperty
{
	DEFINE_CLASS(Node, JingleScript::Object);

	friend GraphPanel;
	friend Graph;

private:
	Graph* m_Graph = nullptr;
	NodeOutConnectionArray* m_OutData = nullptr;

	float m_EditorPositionX;
	float m_EditorPositionY;

	std::unordered_map<OutPin*, std::vector<std::pair<Node*, InPin*>>> m_OutConnections;
	std::unordered_map<InPin*, std::pair<Node*, OutPin*>> m_InConnections;

	std::unordered_map<std::string, InPin*> m_InPins;
	std::unordered_map<std::string, OutPin*> m_OutPins;

	std::vector<InPin*> m_temp_InPins;
	std::vector<OutPin*> m_temp_OutPins;

public:
	Node();
	~Node();

	void OnCreate();
	void OnDelete();

	void OnSerialize();

	void CreateConnection(OutPin* out, std::pair<Node*, InPin*> in);
	void DeleteConnection(OutPin* out, std::pair<Node*, InPin*> in);

	bool InPinSet(const char* name) const;
	bool OutPinSet(const char* name) const;

};

typedef std::pair<std::pair<Node*, OutPin*>, std::pair<Node*, InPin*>> Edge;

class NodeArray : public JingleScript::Array<Node*>
{
	DEFINE_CLASS(NodeArray, JingleScript::Array<Node*>);

public:
	NodeArray() {}
	~NodeArray() {}

};

class NodeOutConnection : public JingleScript::Object, public ObjectProperty
{
	DEFINE_CLASS(NodeOutConnection, JingleScript::Object);

public:
	NodeOutConnection() {}
	~NodeOutConnection() {}

	NodeInConnectionArray* m_InData = nullptr;

};

class NodeOutConnectionArray : public JingleScript::Array<NodeOutConnection*>
{
	DEFINE_CLASS(NodeOutConnectionArray, JingleScript::Array<NodeOutConnection*>);

public:
	NodeOutConnectionArray() {}
	~NodeOutConnectionArray() {}

};

class NodeInConnection : public JingleScript::Object, public ObjectProperty
{
	DEFINE_CLASS(NodeInConnection, JingleScript::Object);

public:
	NodeInConnection() {}
	~NodeInConnection() {}

	std::string Node;
	std::string Pin;

};

class NodeInConnectionArray : public JingleScript::Array<NodeInConnection*>
{
	DEFINE_CLASS(NodeInConnectionArray, JingleScript::Array<NodeInConnection*>);

public:
	NodeInConnectionArray() {}
	~NodeInConnectionArray() {}

};
