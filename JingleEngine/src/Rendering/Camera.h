#pragma once

#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/Config.h"

#include "Input/Binding.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

class Camera;
class CameraType : public EntityType
{
	DEFINE_CLASS(CameraType, EntityType);

public:
	float MovementSpeed;
	float MovementBoostModifier;
	
	float MouseSpeed;

public:
	CameraType() {}

	virtual void Load(Config& config) override;

};

class Camera : public Entity
{
	DEFINE_CLASS(Camera, Entity);

	float m_SpeedCoef = 1.0;

	glm::vec3 m_Position;
	glm::vec3 m_Orientation;

	Binding* Binding_Mouse_Scroll;
	Binding* Binding_Turbo;
	Binding* Binding_Forward;
	Binding* Binding_Backward;
	Binding* Binding_Right;
	Binding* Binding_Left;

	static Camera* s_Current;
	
public:
	Camera() {}

	virtual void OnCreate() override;
	virtual void OnSimulate(double DeltaTime) override;

	static Camera* GetCurrent();

};
