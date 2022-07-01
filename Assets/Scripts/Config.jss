class ConfigAsset extends Asset
{
	@Native
	Config Get();
}

class Config extends Managed
{
}

class ConfigArray extends Config
{
}

class ConfigSection extends Config
{
}

class ConfigValue extends Config
{
}
