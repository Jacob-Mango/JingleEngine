#include "Camera.h"

void CameraType::Load(Config& config)
{
	super::Load(config);

	MovementSpeed = config["movementSpeed"].Float;
	MovementBoostModifier = config["movementBoostModifier"].Float;

	MouseSpeed = config["mouseSpeed"].Float;
}

void Camera::OnCreate()
{
	super::OnCreate();

	GetScene()->SetCamera(this);

	m_Position = GetPosition();
	m_Orientation = GetOrientation();
}

void Camera::OnSimulate(double DeltaTime)
{
	super::OnSimulate(DeltaTime);

	//if (!g_Application->IsDebug())
	//{
	//	SetPosition(m_Position);
	//	SetOrientation(m_Orientation);
	//	return;
	//}

	glm::vec3 position = GetPosition();
	glm::vec3 orientation = GetOrientation();

	if (BindingManager::IsMouseLocked())
	{
		auto [mouseX, mouseY] = BindingManager::GetMouseDelta();

		orientation.x += float(mouseX * DeltaTime * GetType()->MouseSpeed);
		orientation.y -= float(mouseY * DeltaTime * GetType()->MouseSpeed);
		orientation.z = 0;

		if (orientation.y < -89.0)
			orientation.y = -89.0;
		if (orientation.y > 89.0)
			orientation.y = 89.0;

		float scrollAmt = BindingManager::GetValue("mouse_scroll");
		m_SpeedCoef += float(scrollAmt * DeltaTime * (m_SpeedCoef * 20.0));
	}

	m_SpeedCoef = glm::clamp(m_SpeedCoef, 0.01f, 500.0f);

	float speed = GetType()->MovementSpeed;

	if (BindingManager::IsMouseLocked() && BindingManager::Get("turbo") >= BindingState::PRESSED)
	{
		speed *= GetType()->MovementBoostModifier;
	}

	speed = speed * m_SpeedCoef;

	float forward = 0;
	float strafe = 0;

	if (BindingManager::IsMouseLocked() && BindingManager::Get("forward") >= BindingState::PRESSED)
	{
		forward = speed;
	}

	if (BindingManager::IsMouseLocked() && BindingManager::Get("backward") >= BindingState::PRESSED)
	{
		forward = -speed;
	}

	if (BindingManager::IsMouseLocked() && BindingManager::Get("right") >= BindingState::PRESSED)
	{
		strafe = -speed;
	}

	if (BindingManager::IsMouseLocked() && BindingManager::Get("left") >= BindingState::PRESSED)
	{
		strafe = speed;
	}

	SetOrientation(orientation);

	position += GetRightDirection() * strafe * (float)DeltaTime;
	position += GetForwardDirection() * forward * (float)DeltaTime;

	SetPosition(position);
}
