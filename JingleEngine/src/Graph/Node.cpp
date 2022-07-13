#include "Graph/Node.h"

#include "Graph/Pin.h"
#include "Graph/Graph.h"

BEGIN_CLASS_LINK(Node)
	LINK_NAMED_VARIABLE(ConnectionsData, m_ConnectionsData);
	LINK_NAMED_VARIABLE(EditorPositionX, m_EditorPositionX);
	LINK_NAMED_VARIABLE(EditorPositionY, m_EditorPositionY);
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

BEGIN_CLASS_LINK(NodeArray)
	LINK_CONSTRUCTOR();
	Array<Node*>::ScriptRegister(type);
END_CLASS_LINK()

BEGIN_CLASS_LINK(NodeConnection)
	LINK_VARIABLE(Node);
	LINK_VARIABLE(Pin);
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

BEGIN_CLASS_LINK(NodeConnectionArray)
	LINK_CONSTRUCTOR();
	Array<NodeConnection*>::ScriptRegister(type);
END_CLASS_LINK()

template<typename T>
static T* GetAttribute(const JingleScript::Type::VariableDefinition* variable)
{
	JS_TRACE(Tracers::Property);

	for (auto& attributeBase : variable->Attributes)
	{
		if (attributeBase->GetType()->IsInherited(T::StaticType()))
		{
			return static_cast<T*>(attributeBase);
		}
	}

	return nullptr;
}

template<typename T>
static T* GetAttribute(Node* node, const std::string& name)
{
	JS_TRACE(Tracers::Property);

	return GetAttribute<T>(&node->GetType()->GetVariableDefinition(name));
}

Node::Node()
{
	m_EditorPositionX = 0;
	m_EditorPositionY = 0;
}

Node::~Node()
{
}

void Node::OnCreate()
{
	for (auto& variable : GetType()->GetVariables())
	{
		InPin* inPin = GetAttribute<InPin>(variable);
		OutPin* outPin = GetAttribute<OutPin>(variable);

		if (inPin && outPin)
		{
			JS_ERROR("Variable {} set as both in and out, skipping.", variable->Name);
			continue;
		}

		if (inPin)
		{
			m_InPins[variable->Name] = inPin;
			m_temp_InPins.push_back(inPin);
		}

		if (outPin)
		{
			m_OutPins[variable->Name] = outPin;
			m_temp_OutPins.push_back(outPin);
		}
	}

	for (auto& connection : *m_ConnectionsData)
	{
		OutPin* outPin = GetAttribute<OutPin>(this, connection->GetName());
		if (!outPin)
		{
			continue;
		}

		Node* inNode = nullptr;

		for (auto& node : *m_Graph->m_Nodes)
		{
			if (node->GetName() == connection->Node)
			{
				inNode = node;
			}
		}

		if (!inNode)
		{
			JS_ERROR("{}: Failed to find node '{}'", GetName(), connection->Node);
			continue;
		}

		InPin* inPin = GetAttribute<InPin>(inNode, connection->Pin);
		if (!inPin)
		{
			JS_ERROR("{}: Failed to find pin '{}' in node '{}'", GetName(), connection->Pin, connection->Node);
			continue;
		}

		CreateConnection(outPin, { inNode, inPin });
	}
}

void Node::OnSerialize()
{
	//! TODO: mem leak, delete connections
	m_ConnectionsData->Clear();
	
	for (auto& connection : m_OutConnections)
	{
		NodeConnection* connectionData = JingleScript::NewObject<NodeConnection>("NodeConnection");
		void* dta = (void*)connectionData;
		connectionData->OnDeserialize(nullptr, dta);

		connectionData->SetName(connection.first->GetName());
		connectionData->Node = connection.second.first->GetName();
		connectionData->Pin = connection.second.second->GetName();

		m_ConnectionsData->Insert(connectionData);
	}
}

void Node::CreateConnection(OutPin* out, std::pair<Node*, InPin*> in)
{
	m_OutConnections[out] = { in.first, in.second };
	in.first->m_InConnections[in.second] = { this, out };
}

void Node::DeleteConnection(OutPin* out)
{
	auto itOut = m_OutConnections.find(out);
	if (itOut == m_OutConnections.end())
	{
		return;
	}

	Node* inNode = itOut->second.first;
	InPin* in = itOut->second.second;

	auto itIn = inNode->m_InConnections.find(in);
	if (itIn != inNode->m_InConnections.end())
	{
		inNode->m_InConnections.erase(itIn);
	}

	m_OutConnections.erase(itOut);
}

bool Node::InPinSet(const char* name) const
{
	return m_InPins.find(name) != m_InPins.end();
}

bool Node::OutPinSet(const char* name) const
{
	return m_OutPins.find(name) != m_OutPins.end();
}
