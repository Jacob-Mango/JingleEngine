#pragma once

#include "Application.h"
#include "Config.h"
#include "Entity.h"
#include "Scene.h"

class Debug;
class DebugType : public EntityType
{
	ENTITY_TYPE(Debug, Entity)
public:
};

class Debug : public Entity
{
	ENTITY(Debug, Entity)

public:
	virtual void OnSimulate(double DeltaTime) override;
};
