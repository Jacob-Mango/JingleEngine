class Entity
{
	@Property("OnSerializeComponents", "OnDeserializeComponents")
	ComponentArray Components;

	@Property("OnSerializeChildren", "OnDeserializeChildren")
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
	void RemoveChild(Entity child);

	@Native
	Entity GetParent();
}

class Component
{

}

class MeshComponent extends Component
{
	@Property
	Mesh Model;

	@Property
	Material MaterialOverride;
}
