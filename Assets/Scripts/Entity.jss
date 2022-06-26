class Entity
{
	@ArrayProperty("Component")
	ComponentArray Components;
}

class Component
{

}

struct ComponentArray extends Array { }

class MeshComponent extends Component
{
	@Property
	Mesh Model;

	@Property
	Material MaterialOverride;
}
