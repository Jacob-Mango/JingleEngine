#include "Editor/Panels/EntityPropertiesPanel.h"

#include "Core/Application.h"

#include "Editor/EditorUI.h"

#include "Entities/Components/MeshComponent.h"

BEGIN_CLASS_LINK(EntityPropertiesPanel);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

void EntityPropertiesPanel::OnBeginRender(double DeltaTime)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

void EntityPropertiesPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}

void EntityPropertiesPanel::OnRender(double DeltaTime)
{
	Entity* entity = GetEditor()->GetSelectedEntity();
	if (entity == nullptr)
	{
		return;
	}

	EditorUI::ResetContext();

	if (!ImGui::BeginTable("EntityPropertiesPanel", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{
		return;
	}

	entity->Editor_Render();

	ImGui::EndTable();
}
