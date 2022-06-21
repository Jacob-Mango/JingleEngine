#pragma once

#include "Core/Application.h"
#include "Core/Core.h"

#include "Scene/Scene.h"

/*
class Scene;
class Planet;

class PlanetQuad : public MeshEntity
{
	DEFINE_CLASS(PlanetQuad, MeshEntity);

	friend class Planet;

	Planet* m_Planet;

	PlanetQuad* m_ParentQuad;
	std::vector<PlanetQuad*> m_ChildQuads;
	
	double m_BoundingSphereRadius;

	unsigned int m_Quadrant;
	unsigned long long m_Depth;

	glm::dmat3 m_QuadFace;

public:
	PlanetQuad() {}

	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnTick(double DeltaTime) override;

	void SpawnChild(unsigned int quadrant);

	void CreateMesh();

	bool DestroyChildQuads();

	glm::dvec2 GetPlanetRelativePosition();
	glm::dvec2 GetPlanetQuadrantRelativePosition();
	double GetPlanetRelativeScale();
};
*/
