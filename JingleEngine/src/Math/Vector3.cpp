#include "Math/Vector3.h"

Vector3 Vector3::FromString(std::string value)
{
	Vector3 result;
	if (value.empty())
	{
		return result;
	}

	std::istringstream ss(value);
	
	if (!(ss >> result[0]))
	{
		return result;
	}

	if (!(ss >> result[1]))
	{
		return result;
	}

	if (!(ss >> result[2]))
	{
		return result;
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
