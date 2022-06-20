#pragma once

#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/Config.h"

#include "Input/Binding.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

class Camera : public Entity
{
	DEFINE_CLASS(Camera, Entity);

public:
	double MovementSpeed;
	double MovementBoostModifier;
	
	double MouseSpeed;

private:
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
	Camera() {}

	virtual void OnCreate() override;
	virtual void OnTick(double DeltaTime) override;

};
