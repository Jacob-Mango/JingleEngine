class Scene extends ConfigAsset
{
	@Property
	ConfigArray Entities;

	@Native
	Camera GetCamera();
}
