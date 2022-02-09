#pragma once

#include "Core/Application.h"
#include "Core/Config.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

class Camera;
class CameraType : public EntityType
{
	ENTITY_TYPE(Camera, Entity)

public:
	float MovementSpeed;
	float MovementBoostModifier;
	
	float MouseSpeed;
};

class Camera : public Entity
{
	ENTITY(Camera, Entity)

	float m_SpeedCoef = 1.0;

	glm::vec3 m_Position;
	glm::vec3 m_Orientation;
public:

	virtual void OnCreate() override;
	virtual void OnSimulate(double DeltaTime) override;
};
