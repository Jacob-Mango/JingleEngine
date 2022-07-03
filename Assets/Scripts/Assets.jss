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
}

class Material extends ConfigAsset
{
	@Property
	Shader Shader;

	@Property
	MaterialImageArray Images;
}

class Mesh extends Asset
{

}
