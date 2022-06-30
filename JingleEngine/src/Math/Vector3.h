#pragma once

#include "Core/Core.h"

struct Vector3 : public glm::dvec3
{
	DEFINE_STRUCTURE(Vector3);

public:
	Vector3() {}
	Vector3(const Vector3& other) : glm::dvec3(other) {}
	Vector3(const Vector3&& other) : glm::dvec3(other) {}
	Vector3(const glm::dvec3& other) : glm::dvec3(other) {}
	Vector3(const glm::dvec3&& other) : glm::dvec3(other) {}
	Vector3(const glm::vec3& other) : glm::dvec3(other) {}
	Vector3(const glm::vec3&& other) : glm::dvec3(other) {}
	Vector3(const glm::dvec4& other) : glm::dvec3(other) {}
	Vector3(const glm::dvec4&& other) : glm::dvec3(other) {}
	Vector3(const glm::vec4& other) : glm::dvec3(other) {}
	Vector3(const glm::vec4&& other) : glm::dvec3(other) {}

	static Vector3 FromString(std::string value);
	std::string ToString() const;
};
