#pragma once

#include "Core/Application.h"
#include "Core/Config.h"

#include "Scene/MeshEntity.h"
#include "Scene/Scene.h"

class Scene;
class Planet;

class PlanetQuad;
class PlanetQuadType : public MeshEntityType
{
	ENTITY_TYPE(PlanetQuad, MeshEntity)

public:
};

class PlanetQuad : public MeshEntity
{
	ENTITY(PlanetQuad, MeshEntity)

	friend class Planet;

	Ref<Planet> m_Planet;

	PlanetQuad* m_ParentQuad;
	std::vector<Ref<PlanetQuad>> m_ChildQuads;
	
	double m_BoundingSphereRadius;

	unsigned int m_Quadrant;
	unsigned long long m_Depth;

	glm::dmat3 m_QuadFace;

public:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnSimulate(double DeltaTime) override;

	void SpawnChild(unsigned int quadrant);

	void CreateMesh();

	bool DestroyChildQuads();

	glm::dvec2 GetPlanetRelativePosition();
	glm::dvec2 GetPlanetQuadrantRelativePosition();
	double GetPlanetRelativeScale();
};
