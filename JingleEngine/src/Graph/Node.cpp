#include "Graph/Node.h"

#include "Graph/Pin.h"
#include "Graph/Graph.h"

BEGIN_CLASS_LINK(Node)
	LINK_NAMED_VARIABLE(Out, m_OutData);
	LINK_NAMED_VARIABLE(EditorPositionX, m_EditorPositionX);
	LINK_NAMED_VARIABLE(EditorPositionY, m_EditorPositionY);

	LINK_CONSTRUCTOR();
END_CLASS_LINK()

BEGIN_CLASS_LINK(NodeArray)
	LINK_CONSTRUCTOR();
	Array<Node*>::ScriptRegister(type);
END_CLASS_LINK()

BEGIN_CLASS_LINK(NodeOutConnection)
	LINK_NAMED_VARIABLE(In, m_InData);

	LINK_CONSTRUCTOR();
END_CLASS_LINK()

BEGIN_CLASS_LINK(NodeOutConnectionArray)
	LINK_CONSTRUCTOR();
	Array<NodeOutConnection*>::ScriptRegister(type);
END_CLASS_LINK()

BEGIN_CLASS_LINK(NodeInConnection)
	LINK_VARIABLE(Node);
	LINK_VARIABLE(Pin);

	LINK_CONSTRUCTOR();
END_CLASS_LINK()

BEGIN_CLASS_LINK(NodeInConnectionArray)
	LINK_CONSTRUCTOR();
	Array<NodeInConnection*>::ScriptRegister(type);
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

	for (auto& out : *m_OutData)
	{
		OutPin* outPin = GetAttribute<OutPin>(this, out->GetName());
		if (!outPin)
		{
			continue;
		}

		for (auto& in : *out->m_InData)
		{
			Node* inNode = nullptr;

			for (auto& node : *m_Graph->m_Nodes)
			{
				if (node->GetName() == in->Node)
				{
					inNode = node;
				}
			}

			if (!inNode)
			{
				JS_ERROR("{}: Failed to find node '{}'", GetName(), in->Node);
				continue;
			}

			InPin* inPin = GetAttribute<InPin>(inNode, in->Pin);
			if (!inPin)
			{
				JS_ERROR("{}: Failed to find pin '{}' in node '{}'", GetName(), in->Pin, in->Node);
				continue;
			}

			CreateConnection(outPin, { inNode, inPin });
		}
	}
}

void Node::OnDelete()
{
	for (auto& connection : m_OutConnections)
	{
		for (auto& inConnection : connection.second)
		{
			DeleteConnection(connection.first, inConnection);
		}
	}

	for (auto& connection : m_InConnections)
	{
		std::pair<Node*, InPin*> inPin;

		inPin.first = this;
		inPin.second = connection.first;

		Node* node = connection.second.first;
		OutPin* pin = connection.second.second;

		node->DeleteConnection(pin, inPin);
	}
}

void Node::OnSerialize()
{
	//! TODO: mem leak, delete connections
	m_OutData->Clear();

	for (auto& outConnections : m_OutConnections)
	{
		NodeOutConnection* out = JingleScript::NewObject<NodeOutConnection>("NodeOutConnection");

		void* dta = (void*)out;
		out->OnDeserialize(nullptr, dta);

		out->SetName(outConnections.first->GetName());

		int index = 0;

		for (auto& inConnection : outConnections.second)
		{
			NodeInConnection* in = JingleScript::NewObject<NodeInConnection>("NodeInConnection");

			void* dta = (void*)in;
			in->OnDeserialize(nullptr, dta);

			in->Node = inConnection.first->GetName();
			in->Pin = inConnection.second->GetName();
			in->SetName(fmt::format("{}", index++));

			out->m_InData->Insert(in);
		}

		m_OutData->Insert(out);
	}
}

void Node::CreateConnection(OutPin* out, std::pair<Node*, InPin*> in)
{
	Node* inNode = in.first;
	InPin* inPin = in.second;

	auto itIn = inNode->m_InConnections.find(inPin);
	if (itIn != inNode->m_InConnections.end())
	{
		Node* otherNode = itIn->second.first;
		OutPin* otherOutPin = itIn->second.second;

		if (otherNode == in.first && otherOutPin == out)
		{
			//! Already connected
			return;
		}
		else if (otherNode)
		{
			otherNode->DeleteConnection(otherOutPin, { inNode, inPin });
		}
	}

	m_OutConnections[out].push_back({ in.first, in.second });
	in.first->m_InConnections[in.second] = { this, out };
}

void Node::DeleteConnection(OutPin* out, std::pair<Node*, InPin*> in)
{
	auto itOut = m_OutConnections.find(out);
	if (itOut == m_OutConnections.end())
	{
		return;
	}

	auto itIn = std::find(itOut->second.begin(), itOut->second.end(), in);
	if (itIn == itOut->second.end())
	{
		return;
	}

	Node* inNode = itIn->first;
	auto inOutIn = inNode->m_InConnections.find(in.second);
	if (inOutIn != inNode->m_InConnections.end())
	{
		inNode->m_InConnections.erase(inOutIn);
	}

	itOut->second.erase(itIn);
}

bool Node::InPinSet(const char* name) const
{
	return m_InPins.find(name) != m_InPins.end();
}

bool Node::OutPinSet(const char* name) const
{
	return m_OutPins.find(name) != m_OutPins.end();
}
