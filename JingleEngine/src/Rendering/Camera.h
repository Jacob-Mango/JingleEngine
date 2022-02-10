#pragma once

#include "Core/Application.h"
#include "Core/Config.h"

#include "Input/Binding.h"
#include "Input/BindingModule.h"

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

	Binding* Binding_Mouse_Scroll;
	Binding* Binding_Turbo;
	Binding* Binding_Forward;
	Binding* Binding_Backward;
	Binding* Binding_Right;
	Binding* Binding_Left;
public:

	virtual void OnCreate() override;
	virtual void OnSimulate(double DeltaTime) override;
};
