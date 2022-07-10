#pragma once

#include "Core/Core.h"

#include "Config/ConfigAsset.h"

class Node;
class Graph;

class Pin : public JingleScript::Object, public ObjectProperty
{
	DEFINE_CLASS(Pin, JingleScript::Object);

	friend Graph;
	friend ::Node;

private:
	::Node* m_Node;
	Pin* m_Linked;

public:
	Pin();
	~Pin();

};
