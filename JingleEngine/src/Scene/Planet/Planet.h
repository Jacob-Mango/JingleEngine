#pragma once

#include "Core/Application.h"
#include "Core/Core.h"

#include "Scene/Scene.h"

#include "Scene/Planet/PlanetQuad.h"

/*
class Scene;
class PlanetQuad;

class Planet : public MeshEntity
{
	DEFINE_CLASS(Planet, MeshEntity);

	friend PlanetQuad;

public:
	double Radius;
	AssetID QuadType;
	
private:
	std::vector<PlanetQuad*> m_ChildQuads;

public:
	Planet() {}

	virtual void OnCreate() override;
	virtual void OnTick(double DeltaTime) override;

	void CreateFace(glm::dvec3 right, glm::dvec3 up, glm::dvec3 forward);

	glm::dvec3 CubeToSphere(glm::dvec3 point);

	PlanetQuadType& GetQuadType();
};
*/
