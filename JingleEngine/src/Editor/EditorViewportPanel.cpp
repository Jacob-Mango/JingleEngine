#include "Editor/EditorViewportPanel.h"

#include "Core/Application.h"

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
	auto size = ImGui::GetContentRegionAvail();

	m_Viewport->SetScene(Application::Get()->GetScene());

	m_Viewport->Resize(size.x, size.y);

	m_Viewport->Process(DeltaTime);

	m_Viewport->GetTexture()->ImGui(size);
}

std::string EditorViewportPanel::GetTitle() const
{
	return "Viewport";
}
