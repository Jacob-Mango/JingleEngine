class Object
{
	@Native
	string ToString();

	@Native
	Type GetType();

	@Native
	bool operator==(Object other);

	@Native
	bool operator!=(Object other);
}

class Type
{
	@Native
	bool IsInherited(Type other);
}

class Attribute
{

}

class Native extends Attribute
{

}

class null
{
	// All valid native operators are generated at compile time
}

class byte
{
	byte value;

	// All valid native operators are generated at compile time
}

class int
{
	int value;

	// All valid native operators are generated at compile time
}

class bool
{
	bool value;

	// All valid native operators are generated at compile time
}

class float
{
	float value;

	// All valid native operators are generated at compile time
}

class double
{
	double value;

	// All valid native operators are generated at compile time
}

class string
{
	string value;

	// All valid native operators are generated at compile time
}
