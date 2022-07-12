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
