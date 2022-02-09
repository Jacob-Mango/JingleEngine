#include "Frustum.h"

Frustum::Frustum()
{
}

void Frustum::Update(glm::dvec3 position, glm::mat4 orientation)
{
	m_Planes[0] = glm::dvec4(position, 0.0) - glm::dvec4(orientation[0]);
	m_Planes[1] = glm::dvec4(position, 0.0) + glm::dvec4(orientation[0]);
	m_Planes[2] = glm::dvec4(position, 0.0) - glm::dvec4(orientation[2]);
	m_Planes[3] = glm::dvec4(position, 0.0) + glm::dvec4(orientation[2]);
	m_Planes[4] = glm::dvec4(position, 0.0) - glm::dvec4(orientation[1]);
	m_Planes[5] = glm::dvec4(position, 0.0) + glm::dvec4(orientation[1]);

	for (unsigned int i = 0; i < 6; i++)
	{
		const double normalLength = glm::length(glm::dvec3(m_Planes[i].x, m_Planes[i].y, m_Planes[i].z));
		if (normalLength != 0.0)
			m_Planes[i] /= normalLength;
	}
}

double Frustum::DistanceToPlane(unsigned int plane, const glm::dvec3& point) const
{
	return (m_Planes[plane].x * point.x) + (m_Planes[plane].y * point.y) + (m_Planes[plane].z * point.z) + (m_Planes[plane].w);
}

int Frustum::ContainsPoint(const glm::dvec3& point) const
{
	for (unsigned int i = 0; i < 6; i++)
		if (DistanceToPlane(i, point) <= 0.0)
			return Type::OUTSIDE;

	return Type::INSIDE;
}

int Frustum::ContainsSphere(const glm::dvec3& position, double radius) const
{
	int planeCount = 0;

	for (unsigned int i = 0; i < 6; i++)
	{
		const double distance = DistanceToPlane(i, position);
		if (distance <= -radius)
			return Type::OUTSIDE;
		else if (distance > radius)
			planeCount++;
	}

	return planeCount == 6 ? Type::INSIDE : Type::INTERSECTING;
}

int Frustum::ContainsBox(const glm::dvec3& min, const glm::dvec3& max) const
{
	std::vector<glm::dvec3> points;
	for (unsigned int i = 0; i < 8; i++)
		points.push_back(glm::dvec3(i < 4 ? max.x : min.x, i % 4 < 2 ? max.y : min.y, i % 2 ? max.z : min.z));

	return ContainsPolygon(points);
}

int Frustum::ContainsPolygon(const std::vector<glm::dvec3>& points) const
{
	int planeCount = 0;

	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned int pointCount = 0;
		for (unsigned int j = 0; j < points.size(); j++)
			if (DistanceToPlane(i, points[j]) > 0.0)
				pointCount++;

		if (pointCount == 0)
			return Type::OUTSIDE;
		else if (pointCount == points.size())
			planeCount++;
	}

	return planeCount == 6 ? Type::INSIDE : Type::INTERSECTING;
}
