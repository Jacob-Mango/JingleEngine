#include "Editor/SceneHierarchyPanel.h"

#include "Core/Application.h"

#include <imgui.h>

BEGIN_CLASS_LINK(SceneHierarchyPanel);
END_CLASS_LINK();

void SceneHierarchyPanel::OnBeginRender(double DeltaTime)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

void SceneHierarchyPanel::OnRender(double DeltaTime)
{
	if (!ImGui::BeginTable("test", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Hideable))
	{
		return;
	}

	//ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
	//ImGui::TableSetColumnIndex(0);
	//ImGui::Text("Entities");

	Scene* scene = Application::Get()->GetScene();
	for (auto& entity : scene->m_Entities)
	{
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
	}
	
	ImGui::EndTable();
}

void SceneHierarchyPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}
