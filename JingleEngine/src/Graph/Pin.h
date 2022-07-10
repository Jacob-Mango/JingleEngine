#pragma once

#include "Core/Core.h"

#include "Config/ConfigAsset.h"

#include "Property/Property.h"

class Node;
class Graph;

class Pin : public JingleScript::Attribute
{
	DEFINE_CLASS(Pin, JingleScript::Attribute);

	friend Graph;

public:
	Pin();
	~Pin();

};

class InPin : public Pin
{
	DEFINE_CLASS(InPin, Pin);

public:
	InPin();
	~InPin();

};

class OutPin : public Pin
{
	DEFINE_CLASS(OutPin, Pin);

public:
	OutPin();
	~OutPin();

};
