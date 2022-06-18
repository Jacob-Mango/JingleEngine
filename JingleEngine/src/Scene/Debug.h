#pragma once

#include "Core/Core.h"

#include "Core/Application.h"
#include "Core/Config.h"

#include "Scene/MeshEntity.h"

class Debug;
class DebugType : public MeshEntityType
{
	DEFINE_CLASS(DebugType, MeshEntityType);

public:
	DebugType() {}

	virtual void Load(Config& config) override;

};

class Debug : public MeshEntity
{
	DEFINE_CLASS(Debug, MeshEntity);

public:
	Debug() {}

	virtual void OnTick(double DeltaTime) override;

};
