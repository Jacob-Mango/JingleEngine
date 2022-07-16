#pragma once

#include "Core/Core.h"

#include "Config/ConfigAsset.h"

#include "Graph/Pin.h"
#include "Graph/Node.h"

class Graph : public JingleScript::Object, public ObjectProperty
{
	DEFINE_CLASS(Graph, JingleScript::Object);

	friend Node;

private:
	NodeArray* m_Nodes = nullptr;

public:
	Graph();
	~Graph();

public:
	void OnSerializeNodes(Config* cfg);
	void OnDeserializeNodes(Config* cfg);
	void Editor_OnRenderNodes();

	NodeArray& GetNodes() const { return *m_Nodes; }

	virtual JingleScript::Type* GetNodeType() const { return Node::StaticType(); }

	virtual bool OnInsertNode(Node* node);
	virtual bool OnRemoveNode(Node* node);

public:
	template<typename T>
	T* CreateNode(JingleScript::Type* type)
	{
		if (!type->IsInherited(T::StaticType()))
		{
			return nullptr;
		}

		T* node = type->New<T>();
		node->m_Graph = this;
		node->SetName(type->Name());

		if (node->Deserialize(nullptr) && OnInsertNode(node))
		{
			node->OnCreate();

			m_Nodes->Insert(node);
		}
		else
		{
			//todo: delete node, mem leak
		}

		return node;
	}

	template<typename T>
	T* CreateNode()
	{
		return CreateNode(T::StaticType());
	}
	
	bool RemoveNode(Node* node);

};
