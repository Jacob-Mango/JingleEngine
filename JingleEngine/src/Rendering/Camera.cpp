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

	auto bindingModule = Application::Get()->GetModule<BindingModule>();

	Binding_Mouse_Scroll = bindingModule->GetByName("mouse_scroll");
	Binding_Turbo = bindingModule->GetByName("turbo");
	Binding_Forward = bindingModule->GetByName("forward");
	Binding_Backward = bindingModule->GetByName("backward");
	Binding_Right = bindingModule->GetByName("right");
	Binding_Left = bindingModule->GetByName("left");
}

void Camera::OnSimulate(double DeltaTime)
{
	super::OnSimulate(DeltaTime);

	glm::vec3 position = GetPosition();
	glm::vec3 orientation = GetOrientation();

	if (!Input::IsCursorVisible())
	{
		auto [mouseX, mouseY] = Input::GetMouseDelta();

		orientation.x -= float(mouseX * DeltaTime * GetType()->MouseSpeed);
		orientation.y += float(mouseY * DeltaTime * GetType()->MouseSpeed);
		orientation.z = 0;

		if (orientation.y < -89.0)
			orientation.y = -89.0;
		if (orientation.y > 89.0)
			orientation.y = 89.0;

		float scrollAmt = Binding_Mouse_Scroll->GetValue();
		m_SpeedCoef += float(scrollAmt * DeltaTime * (m_SpeedCoef * 20.0));
	}

	m_SpeedCoef = glm::clamp(m_SpeedCoef, 0.01f, 500.0f);

	float speed = GetType()->MovementSpeed;

	if (!Input::IsCursorVisible() && Binding_Turbo->GetState() >= InputState::PRESSED)
	{
		speed *= GetType()->MovementBoostModifier;
	}

	speed = speed * m_SpeedCoef;

	float forward = 0;
	float strafe = 0;

	if (!Input::IsCursorVisible())
	{
		forward = (Binding_Forward->GetValue() * speed) - (Binding_Backward->GetValue() * speed);
		strafe = (Binding_Right->GetValue() * speed) - (Binding_Left->GetValue() * speed);
	}

	SetOrientation(orientation);

	position += GetRightDirection() * strafe * (float)DeltaTime;
	position += GetForwardDirection() * forward * (float)DeltaTime;

	SetPosition(position);
}
