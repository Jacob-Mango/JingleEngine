#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>

#pragma pack(push, 1)
struct Vertex
{
	DEFINE_NAMED_STRUCTURE(Vertex, Vertex);

public:
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec2 UV;

	std::string ToString()
	{
		return "";
	}
	
};
#pragma pack(pop)
