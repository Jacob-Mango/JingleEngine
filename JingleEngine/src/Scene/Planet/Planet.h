#pragma once

#include "Core/Core.h"

#include "Core/Application.h"
#include "Core/Config.h"

#include "Scene/MeshEntity.h"
#include "Scene/Scene.h"

#include "PlanetQuad.h"

class Scene;

class PlanetType : public MeshEntityType
{
	DEFINE_CLASS(PlanetType, MeshEntityType)

public:
	PlanetQuadType* QuadType;
	double Radius;

public:
	PlanetType() {}

	virtual void Load(Config& config) override;

};

class Planet : public MeshEntity
{
	DEFINE_CLASS(Planet, MeshEntity);

	friend class PlanetQuad;

private:
	std::vector<PlanetQuad*> m_ChildQuads;

public:
	Planet() {}

	virtual void OnCreate() override;
	virtual void OnSimulate(double DeltaTime) override;

	void CreateFace(glm::dvec3 right, glm::dvec3 up, glm::dvec3 forward);

	glm::dvec3 CubeToSphere(glm::dvec3 point);

	PlanetQuadType& GetQuadType();
};
