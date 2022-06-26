class Scene extends ConfigAsset
{
	@Property("Entity")
	PropertyArray Entities;

	@Native
	Camera GetCamera();
}
