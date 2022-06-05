#include "Camera.h"

#include "Core/ModuleManager.h"

#include "Input/BindingModule.h"

BEGIN_CLASS_LINK(CameraType)
END_CLASS_LINK()

BEGIN_CLASS_LINK(Camera)
	LINK_NAMED_FUNCTION(GetCurrentCamera, Camera::GetCurrent);
END_CLASS_LINK()

//! HACK
Camera* Camera::s_Current;
Camera* Camera::GetCurrent()
{
	return s_Current;
}

void CameraType::Load(Config& config)
{
	Super::Load(config);

	MovementSpeed = config["movementSpeed"].Float;
	MovementBoostModifier = config["movementBoostModifier"].Float;

	MouseSpeed = config["mouseSpeed"].Float;
}

void Camera::OnCreate()
{
	Super::OnCreate();

	s_Current = this;

	GetScene()->SetCamera(this);

	m_Position = GetPosition();
	m_Orientation = GetOrientation();

	auto bindingModule = ModuleManager::Get<BindingModule>();

	Binding_Mouse_Scroll = bindingModule->GetByName("mouse_scroll");
	Binding_Turbo = bindingModule->GetByName("turbo");
	Binding_Forward = bindingModule->GetByName("forward");
	Binding_Backward = bindingModule->GetByName("backward");
	Binding_Right = bindingModule->GetByName("right");
	Binding_Left = bindingModule->GetByName("left");
}

void Camera::OnSimulate(double DeltaTime)
{
	Super::OnSimulate(DeltaTime);

	auto& type = GetEntityType<CameraType>();

	glm::dvec3 position = GetPosition();
	glm::dvec3 orientation = GetOrientation();

	if (!Input::IsCursorVisible())
	{
		auto [mouseX, mouseY] = Input::GetMouseDelta();

		orientation.x -= double(mouseX * type.MouseSpeed) * DeltaTime;
		orientation.y += double(mouseY * type.MouseSpeed) * DeltaTime;
		orientation.z = 0;

		if (orientation.y < -89.0)
			orientation.y = -89.0;
		if (orientation.y > 89.0)
			orientation.y = 89.0;

		double scrollAmt = Binding_Mouse_Scroll->GetValue();
		m_SpeedCoef += scrollAmt * DeltaTime * (m_SpeedCoef * 20.0);
	}

	m_SpeedCoef = glm::clamp(m_SpeedCoef, 0.01f, 500.0f);

	double speed = type.MovementSpeed;

	if (!Input::IsCursorVisible() && Binding_Turbo->GetState() >= InputState::PRESSED)
	{
		speed *= type.MovementBoostModifier;
	}

	speed = speed * m_SpeedCoef;

	double forward = 0;
	double strafe = 0;

	if (!Input::IsCursorVisible())
	{
		forward = (Binding_Forward->GetValue() * speed) - (Binding_Backward->GetValue() * speed);
		strafe = (Binding_Left->GetValue() * speed) - (Binding_Right->GetValue() * speed);
	}

	SetOrientation(orientation);

	position += GetRightDirection() * strafe * DeltaTime;
	position += GetForwardDirection() * forward * DeltaTime;

	SetPosition(position);
}
