#include "Graph/Node.h"

#include "Graph/Pin.h"
#include "Graph/Graph.h"

BEGIN_CLASS_LINK(Node)
	LINK_NAMED_VARIABLE(ConnectionsData, m_ConnectionsData);
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

		if (inPin)
		{
			m_InPins.push_back(inPin);
		}

		if (outPin)
		{
			m_OutPins.push_back(outPin);
		}
	}

	for (auto& connection : *m_ConnectionsData)
	{
		OutPin* outPin = GetAttribute<OutPin>(this, connection->GetName());
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

		m_Connections.insert({ outPin, { inNode, inPin } });
	}
}
