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
	Attribute()
	{

	}
}

class Native extends Attribute
{
	Native()
	{
		
	}
}

struct null
{
	// All valid native operators are generated at compile time
}

struct byte
{
	byte value;

	// All valid native operators are generated at compile time
}

struct int
{
	int value;

	// All valid native operators are generated at compile time
}

struct bool
{
	bool value;

	// All valid native operators are generated at compile time
}

struct float
{
	float value;

	// All valid native operators are generated at compile time
}

struct double
{
	double value;

	// All valid native operators are generated at compile time
}

struct string
{
	// All valid native operators are generated at compile time
}
