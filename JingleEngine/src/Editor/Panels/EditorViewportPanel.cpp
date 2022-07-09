#include "Editor/Panels/EditorViewportPanel.h"

#include "Core/Application.h"

#include "Editor/Editor.h"

#include "Input/Binding.h"
#include "Input/BindingModule.h"
#include "Input/Input.h"

#include "Rendering/Viewport.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtx/euler_angles.hpp>

class EditorViewport : public Viewport
{
	DEFINE_CLASS(EditorViewport, Viewport);

	double m_MovementSpeed;
	double m_MovementBoostModifier;
	
	double m_MouseSpeed;

	glm::dmat4 m_Transform;

	//! TODO: Binding Module defines these as variables and loads as a config

	Binding* m_Binding_Mouse_Scroll;
	Binding* m_Binding_Turbo;
	Binding* m_Binding_Forward;
	Binding* m_Binding_Backward;
	Binding* m_Binding_Right;
	Binding* m_Binding_Left;

public:
	EditorViewport()
	{
		auto bindingModule = ModuleManager::Get<BindingModule>();

		m_Binding_Mouse_Scroll = bindingModule->GetByName("mouse_scroll");
		m_Binding_Turbo = bindingModule->GetByName("turbo");
		m_Binding_Forward = bindingModule->GetByName("forward");
		m_Binding_Backward = bindingModule->GetByName("backward");
		m_Binding_Right = bindingModule->GetByName("right");
		m_Binding_Left = bindingModule->GetByName("left");

		m_MovementSpeed = 20.0;
		m_MovementBoostModifier = 2;
		m_MouseSpeed = glm::radians(20.0);

		m_Transform = glm::dmat4(1.0);
	}

	virtual void Process(double DeltaTime) override
	{
		bool cameraActive = Input::IsCursorInViewport() && !Input::IsCursorVisible();

		glm::dvec3 orientation;
		glm::dvec3 position;
		
		glm::extractEulerAngleYXZ(m_Transform, orientation.x, orientation.y, orientation.z);
		position = glm::dvec3(m_Transform[3]);

		if (cameraActive)
		{
			auto [mouseX, mouseY] = Input::GetMouseDelta();

			orientation.x -= double(mouseX) * m_MouseSpeed * DeltaTime;
			orientation.y -= double(mouseY) * m_MouseSpeed * DeltaTime;
			orientation.z = 0;

			orientation.y = glm::clamp(orientation.y, glm::radians(-89.0), glm::radians(89.0));
		}

		m_Transform = glm::eulerAngleYXZ(orientation.x, orientation.y, orientation.z);

		if (cameraActive)
		{
			double scrollAmt = m_Binding_Mouse_Scroll->GetValue();
			double speed = m_MovementSpeed * glm::clamp(1.0 + (scrollAmt * DeltaTime * 20.0), 0.01, 500.0);

			if (m_Binding_Turbo->GetState() >= InputState::PRESSED)
			{
				speed *= m_MovementBoostModifier;
			}

			double forward = (m_Binding_Forward->GetValue() * speed) - (m_Binding_Backward->GetValue() * speed);
			double strafe = (m_Binding_Left->GetValue() * speed) - (m_Binding_Right->GetValue() * speed);

			position -= glm::dvec3(m_Transform[0]) * strafe * DeltaTime;
			position -= glm::dvec3(m_Transform[2]) * forward * DeltaTime;
		}

		m_Transform[3] = glm::dvec4(position, 1.0);
		m_CameraPosition = position;

		m_ViewMatrix = glm::inverse(m_Transform);

		Super::Process(DeltaTime);
	}
};

BEGIN_CLASS_LINK(EditorViewport);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

BEGIN_CLASS_LINK(EditorViewportPanel);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

EditorViewportPanel::EditorViewportPanel()
{
	m_Viewport = Viewport::Create(EditorViewport::StaticName(), nullptr, 1, 1);
}

void EditorViewportPanel::OnBeginRender(double DeltaTime)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

void EditorViewportPanel::OnRender(double DeltaTime)
{
	auto toVec2 = [](std::pair<int, int> val) { 
			return ImVec2(val.first, val.second); 
		};

	ImGuiIO& io = ImGui::GetIO();

	bool hovered = ImGui::IsWindowHovered();
	bool focused = ImGui::IsWindowFocused();

	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 position = ImGui::GetWindowPos();

	Entity* entity = GetEditor()->GetEntity();

	m_Viewport->SetScene(entity);

	m_Viewport->Resize((unsigned int)viewportSize.x, (unsigned int)viewportSize.y);

	m_Viewport->Process(DeltaTime);

	m_Viewport->GetTexture()->ImGui(viewportSize);

	ImVec2 min = { position.x,					position.y + 20.0f };
	ImVec2 max = { position.x + windowSize.x,	position.y + windowSize.y };

	bool cursorInViewport = true;
	cursorInViewport &= focused;

	ImVec2 mousePosition = toVec2(Input::GetMousePosition());

	ImRect rect(min, max);
	cursorInViewport &= rect.Contains(mousePosition);

	cursorInViewport = false;

	if (cursorInViewport)
	{
		std::pair<int, int> viewportMiddle = { position.x + (windowSize.x / 2), position.y + (windowSize.y / 2) };
		Input::SetViewportMiddle(viewportMiddle);
	}
	else
	{
		Input::ShowCursor(true);
		Input::ClearViewport();
	}
}

void EditorViewportPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}
