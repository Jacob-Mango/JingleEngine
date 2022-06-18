#include "Editor/EditorViewportPanel.h"

#include "Core/Application.h"

#include "Input/Input.h"

#include "Rendering/Viewport.h"

#include <imgui.h>

BEGIN_MODULE_LINK(EditorViewportPanel);
END_MODULE_LINK();

void EditorViewportPanel::OnInitialize()
{
	m_Viewport = Viewport::Create(nullptr, 1, 1);
}

void EditorViewportPanel::OnRender(double DeltaTime)
{
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	ImVec2 offset = ImGui::GetCursorPos();
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 position = ImGui::GetWindowPos();

	m_Viewport->SetScene(Application::Get()->GetScene());

	m_Viewport->Resize((unsigned int)viewportSize.x, (unsigned int)viewportSize.y);

	m_Viewport->Process(DeltaTime);

	m_Viewport->GetTexture()->ImGui(viewportSize);

	ImVec2 min = { position.x - offset.x, position.y - offset.y };
	ImVec2 max = { min.x + windowSize.x,  min.y + windowSize.y };

	Input::SetCursorInViewport(ImGui::IsMouseHoveringRect(min, max));
}

std::string EditorViewportPanel::GetTitle() const
{
	return "Viewport";
}
