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
	@Native
	bool operator==(Object other);

	@Native
	bool operator!=(Object other);
}

class byte
{
}

class int
{
	@Native
	int operator*(int other);

	@Native
	int operator/(int other);

	@Native
	int operator+(int other);

	@Native
	int operator-(int other);

	@Native
	int operator%(int other);

	@Native
	bool operator>(int other);

	@Native
	bool operator>=(int other);

	@Native
	bool operator<(int other);

	@Native
	bool operator<=(int other);

	@Native
	bool operator==(int other);

	@Native
	bool operator!=(int other);
}

class bool extends int
{
}

class float
{
	@Native
	float operator*(float other);

	@Native
	float operator/(float other);

	@Native
	float operator+(float other);

	@Native
	float operator-(float other);

	@Native
	bool operator>(float other);

	@Native
	bool operator>=(float other);

	@Native
	bool operator<(float other);

	@Native
	bool operator<=(float other);

	@Native
	bool operator==(float other);

	@Native
	bool operator!=(float other);
}

class double
{
}

class string
{
}
