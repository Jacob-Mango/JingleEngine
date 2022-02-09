#pragma once

#include "Core/Application.h"
#include "Core/Config.h"

#include "Scene/MeshEntity.h"

class Debug;
class DebugType : public MeshEntityType
{
	ENTITY_TYPE(Debug, MeshEntity)
public:
};

class Debug : public MeshEntity
{
	ENTITY(Debug, MeshEntity)

public:
	virtual void OnSimulate(double DeltaTime) override;
};
