#include "Editor/Panels/EntityHierarchyPanel.h"

#include "Core/Application.h"

#include "Editor/Editor.h"
#include "Editor/EditorModule.h"
#include "Editor/EditorUI.h"

#include <imgui.h>

BEGIN_CLASS_LINK(EntityHierarchyPanel);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

void EntityHierarchyPanel::OnBeginRender(double DeltaTime)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

int g_Depth = 0;
float g_DepthIndent = 5.0f;

void EntityHierarchyPanel::OnRender(double DeltaTime)
{
	Entity* entity = GetEditor()->GetEntity();

	g_Depth = 0;

	if (ImGui::BeginTable("Entities", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Hideable))
	{
		ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Entities");

		RenderEntity(entity);

		ImGui::EndTable();
	}
}

void EntityHierarchyPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}

void EntityHierarchyPanel::RenderEntity(Entity* entity)
{
	if (!entity)
	{
		return;
	}

	ScopedIncrement increment(g_Depth);

	ImGui::PushID(PointerToString(entity).c_str());

	std::string name = entity->GetType()->Name();
	bool selected = GetEditorModule()->GetSelectedEntity() == entity;
	bool wasSelected = selected;

	ImGui::TableNextRow(ImGuiTableRowFlags_None);
	ImGui::TableNextColumn();

	ImGui::SetCursorPosX(g_Depth * g_DepthIndent);

	ImGui::Selectable(name.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns);

	if (selected && !wasSelected)
	{
		GetEditorModule()->SelectEntity(entity);
	}

	ImGui::PopID();

	for (auto& child : *entity->m_Children)
	{
		RenderEntity(child);
	}
}
