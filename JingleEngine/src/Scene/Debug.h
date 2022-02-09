#pragma once

#include "Core/Application.h"
#include "Core/Config.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

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
