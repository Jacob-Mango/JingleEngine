class Pin
{

}

class NodeConnection
{
	@Property
	string Node;

	@Property
	string Pin;
}

class Node
{
	@Property
	NodeConnectionArray ConnectionsData;
}

class Graph
{
	@Property(true)
	NodeArray Nodes;
}

class TestNode extends Node
{
	@OutPin
	string Variable;
}

class PrintNode extends Node
{
	@InPin
	string Message;
}
