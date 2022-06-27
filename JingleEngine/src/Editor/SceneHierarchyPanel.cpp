#include "Editor/SceneHierarchyPanel.h"

#include "Core/Application.h"

#include "Property/PropertyArray.h"

#include <imgui.h>

BEGIN_CLASS_LINK(SceneHierarchyPanel);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

void SceneHierarchyPanel::OnBeginRender(double DeltaTime)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

void SceneHierarchyPanel::OnRender(double DeltaTime)
{
	Entity* scene = Application::Get()->GetScene();
	if (!scene)
	{
		return;
	}

	if (ImGui::BeginTable("Entities", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Hideable))
	{
		ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Entities");

		RenderEntity(scene);

		ImGui::EndTable();
	}
}

void SceneHierarchyPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}

void SceneHierarchyPanel::RenderEntity(Entity* entity)
{
	ImGui::PushID(PointerToString(entity).c_str());

	std::string name = entity->GetType()->Name();
	bool selected = GetEditor()->GetSelectedEntity() == entity;
	bool wasSelected = selected;

	ImGui::TableNextRow(ImGuiTableRowFlags_None);
	ImGui::TableNextColumn();

	ImGui::SetCursorPosX(5);

	ImGui::Selectable(name.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns);

	if (selected && !wasSelected)
	{
		GetEditor()->SelectEntity(entity);
	}

	ImGui::PopID();

	for (auto& child : entity->m_Children)
	{
		RenderEntity(child);
	}
}
