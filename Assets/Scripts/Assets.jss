class AssetModule extends Module
{

}

class Asset
{

}

class Image extends Asset
{

}

class Shader extends Asset
{

}

class MaterialImage
{
	@Property
	Image File;

	@Property
	string Name;
}

class Material extends Asset
{
	@Property
	Shader Shader;

	@ArrayProperty("MaterialImage")
	MaterialImageArray Images;
}

class Mesh extends Asset
{

}

class ConfigAsset extends Asset
{
	
}

class EntityFile extends ConfigAsset
{
	
}

class Scene extends ConfigAsset
{
	@ArrayProperty("Entity")
	EntityArray Entities;

	@Native
	Camera GetCamera();
}
