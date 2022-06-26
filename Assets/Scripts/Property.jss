class Property extends Attribute
{
	@Native
	Property();
}

class ArrayProperty extends Property
{
	@Native
	ArrayProperty(string type);
}

class PropertyBase
{
}

class PropertyArray extends PropertyBase
{

}

class PropertyAsset extends PropertyBase
{

}

class PropertyConfig extends PropertyBase
{

}

class PropertyItem extends PropertyBase
{

}

class PropertyObject extends PropertyBase
{

}

class PropertyProperty extends PropertyBase
{

}
