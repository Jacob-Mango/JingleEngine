#include "Math/Transform.h"

#include "Math/Vector3.h"

#include <glm/gtx/euler_angles.hpp>

Transform FromString(std::string value)
{
	std::istringstream ss(value);

	Vector3 position;
	ss >> position[0];
	ss >> position[1];
	ss >> position[2];

	Vector3 orientation;
	ss >> orientation[0];
	ss >> orientation[1];
	ss >> orientation[2];

	Transform result;
	result.SetPosition(position);
	result.SetOrientation(orientation);
	return result;
}

std::string Transform::ToString() const
{
	return fmt::format("{} {}", GetPosition().ToString(), GetOrientation().ToString());
}

Vector3 Transform::GetPosition() const
{
	return Vector3((*this)[3]);
}

void Transform::SetPosition(const Vector3& position)
{
	(*this)[3][0] = position[0];
	(*this)[3][1] = position[1];
	(*this)[3][2] = position[2];
}

Vector3 Transform::GetOrientation() const
{
	glm::vec3 orientation;
	glm::extractEulerAngleYXZ(glm::mat4(*this), orientation.x, orientation.y, orientation.z);
	return Vector3(glm::degrees(orientation));
}

void Transform::SetOrientation(const Vector3& orientation)
{
	glm::dmat4 trans = glm::eulerAngleYXZ(glm::radians(orientation.x), glm::radians(orientation.y), glm::radians(orientation.z));
	trans[3] = (*this)[3];
	*(glm::dmat4*)this = trans;
}

BEGIN_STRUCTURE_LINK(Transform);
	LINK_CONSTRUCTOR();
	LINK_FUNCTION(FromString);
	LINK_METHOD(ToString);
END_STRUCTURE_LINK();
