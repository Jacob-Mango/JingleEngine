#pragma once

#include "Core/Core.h"

struct Vector3;

struct Transform : public glm::dmat4
{
	DEFINE_STRUCTURE(Transform);

public:
	Transform() : glm::dmat4() {}
	Transform(const glm::dmat4& other) : glm::dmat4(other) {}
	Transform(const glm::dmat4&& other) : glm::dmat4(other) {}

	std::string ToString() const;

	Vector3 GetPosition() const;
	void SetPosition(const Vector3& position);

	Vector3 GetOrientation() const;
	void SetOrientation(const Vector3& orientation);

};
