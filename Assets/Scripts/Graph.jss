class Pin
{

}

class NodeInConnection
{
	@Property
	string Node;

	@Property
	string Pin;
}

class NodeOutConnection
{
	@Property
	NodeInConnectionArray In;
}

class Node
{
	@Property
	NodeOutConnectionArray Out;

	@Property
	float EditorPositionX;
	
	@Property
	float EditorPositionY;
}

class Graph
{
	@Property(true)
	NodeArray Nodes;
}
