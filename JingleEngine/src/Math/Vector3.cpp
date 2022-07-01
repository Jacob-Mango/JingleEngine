#include "Math/Vector3.h"

Vector3 Vector3::FromString(std::string value)
{
	if (value.empty())
	{
		return Vector3(0);
	}

	Vector3 result;
	std::istringstream ss(value);
	
	if (!(ss >> result[0]))
	{
		return Vector3(0);
	}

	if (!(ss >> result[1]))
	{
		return Vector3(0);
	}

	if (!(ss >> result[2]))
	{
		return Vector3(0);
	}

	return result;
}

std::string Vector3::ToString() const
{
	return fmt::format("{} {} {}", (*this)[0], (*this)[1], (*this)[2]);
}

BEGIN_STRUCTURE_LINK(Vector3);
	LINK_CONSTRUCTOR();
	LINK_FUNCTION(FromString);
	LINK_METHOD(ToString);
END_STRUCTURE_LINK();
