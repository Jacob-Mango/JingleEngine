#pragma once

#include "Core/Application.h"
#include "Core/Config.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include "PlanetQuad.h"

class Scene;

class PlanetType : public EntityType
{
	ENTITY_TYPE(Planet, Entity)

public:
	Ref<PlanetQuadType> QuadType;
	double Radius;
};

class Planet : public Entity
{
	ENTITY(Planet, Entity)

	friend class PlanetQuad;

private:
	std::vector<Ref<PlanetQuad>> m_ChildQuads;

public:
	ENTITY_CONSTRUCTOR(Planet)

	virtual void OnCreate() override;
	virtual void OnSimulate(double DeltaTime) override;

	void CreateFace(glm::dvec3 right, glm::dvec3 up, glm::dvec3 forward);

	glm::dvec3 CubeToSphere(glm::dvec3 point);

	Ref<PlanetQuadType> GetQuadType();
};
