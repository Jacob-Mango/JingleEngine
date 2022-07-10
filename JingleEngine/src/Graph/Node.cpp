#include "Graph/Node.h"

#include "Graph/Pin.h"

BEGIN_CLASS_LINK(Node)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

BEGIN_CLASS_LINK(NodeArray)
	LINK_CONSTRUCTOR();
END_CLASS_LINK()

Node::Node()
{
}

Node::~Node()
{
}

void Node::OnCreate()
{
	auto variables = GetType()->GetVariables();
	for (auto& variable : variables)
	{
		JingleScript::Type* type = variable->Type;
		if (!type->IsInherited(Pin::StaticType()))
		{
			continue;
		}

		void* data = (void*)((char*)this + variable->Offset);
		Pin* pin = static_cast<Pin*>(data);
		if (!pin)
		{
			JS_ERROR("Pin {} null", variable->Name);
			continue;
		}

		pin->m_Node = this;
	}
}
