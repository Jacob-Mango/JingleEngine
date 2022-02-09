#include "Planet.h"

#include "Scene/Scene.h"

#include "Rendering/Renderer.h"

void PlanetType::Load(Config& config)
{
	super::Load(config);

	QuadType = g_Application->GetEntityType(config["quadType"].String).As<PlanetQuadType>();
	Radius = config["radius"].Float;
}

void Planet::OnCreate()
{
	CreateFace(glm::dvec3( 1, 0, 0), glm::dvec3(0,  1, 0), glm::dvec3(0, 0,  1));
	CreateFace(glm::dvec3(-1, 0, 0), glm::dvec3(0, -1, 0), glm::dvec3(0, 0, -1));

	CreateFace(glm::dvec3( 1, 0, 0), glm::dvec3(0, 0,  1), glm::dvec3(0,  1, 0));
	CreateFace(glm::dvec3(-1, 0, 0), glm::dvec3(0, 0, -1), glm::dvec3(0, -1, 0));

	CreateFace(glm::dvec3(0,  1, 0), glm::dvec3( 1, 0, 0), glm::dvec3(0, 0,  1));
	CreateFace(glm::dvec3(0, -1, 0), glm::dvec3(-1, 0, 0), glm::dvec3(0, 0, -1));
}

void Planet::OnSimulate(double DeltaTime)
{
}

void Planet::CreateFace(glm::dvec3 right, glm::dvec3 up, glm::dvec3 forward)
{
	Ref<PlanetQuad> quad = GetScene()->SpawnEntity<PlanetQuad>(GetQuadType().Get());
	AddChild(quad.As<Entity>());

	quad->m_Quadrant = 0;
	quad->m_Depth = 0;
	quad->m_Planet = this;
	quad->m_QuadFace = glm::dmat3(right, up, forward);
	quad->CreateMesh();

	m_ChildQuads.push_back(quad);
}

glm::dvec3 Planet::CubeToSphere(glm::dvec3 point)
{
	const double x2 = point.x * point.x;
	const double y2 = point.y * point.y;
	const double z2 = point.z * point.z;
	glm::dvec3 result = glm::dvec3(0);
	result.x = point.x * sqrt(1.0 - ((y2 + z2) * 0.5) + (y2 * z2 * 0.33333333333333333333));
	result.y = point.y * sqrt(1.0 - ((z2 + x2) * 0.5) + (z2 * x2 * 0.33333333333333333333));
	result.z = point.z * sqrt(1.0 - ((x2 + y2) * 0.5) + (x2 * y2 * 0.33333333333333333333));
	return result;
}

Ref<PlanetQuadType> Planet::GetQuadType()
{
	return GetType()->QuadType;
}
