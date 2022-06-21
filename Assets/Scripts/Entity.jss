class Entity
{
	@ArrayProperty("Component")
	ComponentArray Components;
}

struct EntityArray extends Array { }

class Component
{

}

struct ComponentArray extends Array { }

class MeshComponent extends Component
{
	@Property
	string Model;

	@Property
	string Material;
}
