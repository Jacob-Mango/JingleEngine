#pragma once

#include "Core.h"

class Frustum
{
private:
	glm::dvec4 m_Planes[6];

public:
	Frustum();

	enum Type
	{
		OUTSIDE = 0,
		INSIDE = 1,
		INTERSECTING = 2
	};

	void Update(glm::dvec3 position, glm::mat4 orientation);

	double DistanceToPlane(unsigned int plane, const glm::dvec3& point) const;

	int ContainsPoint(const glm::dvec3& point) const;
	int ContainsSphere(const glm::dvec3& position, double radius) const;
	int ContainsBox(const glm::dvec3& min, const glm::dvec3& max) const;
	int ContainsPolygon(const std::vector<glm::dvec3>& points) const;
};
