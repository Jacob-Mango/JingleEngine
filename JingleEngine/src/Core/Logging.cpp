#include "Core/Core.h"

std::ostream& operator<<(std::ostream& os, const glm::vec2& value)
{
	os << value.x << ", " << value.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::dvec2& value)
{
	os << value.x << ", " << value.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& value)
{
	os << value.x << ", " << value.y << ", " << value.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::dvec3& value)
{
	os << value.x << ", " << value.y << ", " << value.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec4& value)
{
	os << value.x << ", " << value.y << ", " << value.z << ", " << value.w;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::dvec4& value)
{
	os << value.x << ", " << value.y << ", " << value.z << ", " << value.w;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::mat3& value)
{
	os << value[0] << ", " << value[1] << ", " << value[2];
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::mat4& value)
{
	os << value[0] << ", " << value[1] << ", " << value[2] << ", " << value[3];
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::quat& value)
{
	os << value.x << ", " << value.y << ", " << value.z << ", " << value.w;
	return os;
}
