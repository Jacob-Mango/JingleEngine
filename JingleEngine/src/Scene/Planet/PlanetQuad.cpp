#include "PlanetQuad.h"

#include "Planet.h"

#include "Scene/Scene.h"

#include "Rendering/Renderer.h"

#define D_DIST(a, b) (((double)b - (double)a) * ((double)b - (double)a))
#define D_LEN(a) (a * a)
#define V_DIST(a, b) D_DIST(a.x, b.x) + D_DIST(a.y, b.y) + D_DIST(a.z, b.z)
#define V_LEN(a) D_LEN(a.x) + D_LEN(a.y) + D_LEN(a.z)

BEGIN_CLASS_LINK(PlanetQuadType)
END_CLASS_LINK()

BEGIN_CLASS_LINK(PlanetQuad)
END_CLASS_LINK()

void PlanetQuadType::Load(Config& config)
{
	Super::Load(config);
}

void PlanetQuad::OnCreate()
{
	Super::OnCreate();

}

void PlanetQuad::OnDestroy()
{
	Super::OnDestroy();

	DestroyChildQuads();
}

void PlanetQuad::OnSimulate(double DeltaTime)
{
	glm::dvec3 position = GetWorldTransform()[3];
	glm::vec3 cameraPosition = GetScene()->GetCamera()->GetPosition();

	double distance = V_DIST(cameraPosition, position);
	double boundingRadius = (m_BoundingSphereRadius * m_BoundingSphereRadius) * 2.5;

	if (m_IsVisible)
	{
		if (distance < boundingRadius && m_Depth < 5)
		{
			SpawnChild(0b01);
			SpawnChild(0b10);
			SpawnChild(0b00);
			SpawnChild(0b11);
		}
	}
	else
	{
		if (distance > boundingRadius)
		{
			DestroyChildQuads();
		}
	}

	m_IsVisible = m_ChildQuads.size() == 0;
}

bool PlanetQuad::DestroyChildQuads()
{
	if (m_ChildQuads.size() == 0) return false;

	for (int i = m_ChildQuads.size() - 1; i >= 0; i--)
	{
		m_ChildQuads[i]->Delete();
	}

	m_ChildQuads.clear();
	return true;
}

#include <glm/gtc/noise.hpp>

const float PERLIN_OCTAVES = 1;
const float PERLIN_PERSISTENCE = 1;

const float ELEVATION_SCALE = 1.0;

double noise(glm::dvec3 position) {
	double total = 0;
	double freq = 10.0;
	int octaves = PERLIN_OCTAVES;
	double persistence = PERLIN_PERSISTENCE;
	double amplitude = 1;
	double maxValue = 0;

	for (int i = 0; i < octaves; i++)
	{
		glm::dvec3 v(position.x * freq, position.y * freq, position.z * freq);
		double value = glm::perlin(v);
		value += 0.5;
		value /= 2.0;

		total += value * amplitude;

		maxValue += amplitude;

		amplitude *= persistence;
		freq *= 2;
	}

	total *= ELEVATION_SCALE / maxValue;

	return total;
}

void PlanetQuad::SpawnChild(unsigned int quadrant)
{
	PlanetQuad* quad = GetScene()->SpawnEntity<PlanetQuad>(&GetEntityType<PlanetQuadType>());
	AddChild(quad);

	quad->m_Quadrant = quadrant;
	quad->m_Depth = m_Depth + 1;
	quad->m_Planet = m_Planet;
	quad->m_ParentQuad = this;
	quad->m_QuadFace = m_QuadFace;
	m_ChildQuads.push_back(quad);

	quad->CreateMesh();
}

void PlanetQuad::CreateMesh()
{
	auto& planetType = m_Planet->GetEntityType<PlanetType>();
	auto& type = GetEntityType<PlanetQuadType>();

	double radius = planetType.Radius;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::dvec3> positions;

	unsigned int offset = 0;

	unsigned int vertexCount = 16 + offset + offset + 1;
	double vertexCountHalf = 16 / 2;

	double quadScale = GetPlanetRelativeScale();
	glm::dvec2 quadPosition = GetPlanetRelativePosition();

	glm::dvec3 right = m_QuadFace[0];
	glm::dvec3 up = m_QuadFace[1];
	glm::dvec3 forward = m_QuadFace[2];

	bool isFlipped = right[0] < -0.5 || up[0] < -0.5 || forward[0] < -0.5;

	for (int x = 0; x < vertexCount; x++)
	{
		for (int z = 0; z < vertexCount; z++)
		{
			Vertex vertex = Vertex();

			glm::dvec2 relativePosition = (glm::dvec2(x - vertexCountHalf - offset, z - vertexCountHalf - offset) * quadScale / vertexCountHalf) + quadPosition;
			glm::dvec3 position = (right * relativePosition.x) + (forward * relativePosition.y) + up;
			position = m_Planet->CubeToSphere(position);

			vertex.Normal = position;
			vertex.UV = glm::abs(relativePosition);

			double r = radius;
			
			if (offset != 0 && (x == 0 || z == 0 || x == vertexCount - 1 || z == vertexCount - 1)) r -= 1.0 / quadScale;
			position *= r;

			positions.push_back(position);

			vertices.push_back(vertex);
		}
	}

	glm::dvec3 center = m_Planet->CubeToSphere((right * quadPosition.x) + (forward * quadPosition.y) + up) * radius;
	m_BoundingSphereRadius = 0;

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		glm::dvec3 position = positions[i] - center;

		m_BoundingSphereRadius = glm::max(m_BoundingSphereRadius, V_LEN(position));

		vertices[i].Position = glm::vec3(position);
	}

	m_BoundingSphereRadius = sqrt(m_BoundingSphereRadius);

	for (unsigned int x = 0; x < vertexCount - 1; x++)
	{
		for (unsigned int z = 0; z < vertexCount - 1; z++)
		{
			int v0 = ((x + 0) * vertexCount) + (z + 0);
			int v1 = ((x + 0) * vertexCount) + (z + 1);
			int v2 = ((x + 1) * vertexCount) + (z + 0);
			int v3 = ((x + 1) * vertexCount) + (z + 1);

			if (isFlipped)
			{
				indices.push_back(v1);
				indices.push_back(v2);
				indices.push_back(v3);

				indices.push_back(v0);
				indices.push_back(v2);
				indices.push_back(v1);
			}
			else
			{
				indices.push_back(v1);
				indices.push_back(v2);
				indices.push_back(v0);

				indices.push_back(v3);
				indices.push_back(v2);
				indices.push_back(v1);
			}
		}
	}

	if (m_ParentQuad)
	{
		glm::dvec2 ourQuadrant = GetPlanetRelativePosition();
		glm::dvec3 ourWorldPos = m_Planet->CubeToSphere((right * ourQuadrant.x) + (forward * ourQuadrant.y) + up) * radius;

		glm::dvec2 parentQuadrant = m_ParentQuad->GetPlanetRelativePosition();
		glm::dvec3 parentWorldPos = m_Planet->CubeToSphere((right * parentQuadrant.x) + (forward * parentQuadrant.y) + up) * radius;

		glm::dvec3 relativePos = ourWorldPos - parentWorldPos;

		SetPosition(relativePos);
	}
	else
	{
		SetPosition(up * radius);
	}

	//m_Mesh = new Mesh(type.Material, vertices, indices);
}

glm::dvec2 PlanetQuad::GetPlanetRelativePosition()
{
	if (!m_ParentQuad) return glm::dvec2(0, 0);

	glm::dvec2 rel(0, 0);
	rel.x = ((m_Quadrant >> 0) & 1) == 1 ? 1 : -1;
	rel.y = ((m_Quadrant >> 1) & 1) == 1 ? 1 : -1;
	return (rel * GetPlanetRelativeScale()) + m_ParentQuad->GetPlanetRelativePosition();
}

glm::dvec2 PlanetQuad::GetPlanetQuadrantRelativePosition()
{
	if (!m_ParentQuad) return glm::dvec2(0, 0);

	glm::dvec2 rel(0, 0);
	rel.x = ((m_Quadrant >> 0) & 1) == 1 ? 1 : -1;
	rel.y = ((m_Quadrant >> 1) & 1) == 1 ? 1 : -1;
	return rel * GetPlanetRelativeScale();
}

double PlanetQuad::GetPlanetRelativeScale()
{
	return 1.0 / (double)(((unsigned long long)1) << ((unsigned long long)m_Depth));
}
