class Entity
{
	@ArrayProperty("Component")
	ComponentArray Components;

	@ArrayProperty("Entity", "InitChild")
	EntityArray Children;

	@Native
	void OnCreate();

	@Native
	void OnDestroy();

	@Native
	void OnTick(double DeltaTime);

	@Native
	void AddChild(Entity child);

	@Native
	/*private*/ void InitChild(Entity child);

	@Native
	void RemoveChild(Entity child);

	@Native
	Entity GetParent();
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
