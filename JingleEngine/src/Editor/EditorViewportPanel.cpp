#include "Editor/EditorViewportPanel.h"

#include "Core/Application.h"

#include "Input/Input.h"

#include "Rendering/Viewport.h"

#include <imgui.h>
#include <imgui_internal.h>

BEGIN_CLASS_LINK(EditorViewportPanel);
END_CLASS_LINK();

EditorViewportPanel::EditorViewportPanel()
{
	m_Viewport = Viewport::Create(nullptr, 1, 1);
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

	m_Viewport->SetScene(Application::Get()->GetScene());

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
