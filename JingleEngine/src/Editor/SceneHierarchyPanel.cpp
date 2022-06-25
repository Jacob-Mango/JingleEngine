#include "Editor/SceneHierarchyPanel.h"

#include "Core/Application.h"

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
	Scene* scene = Application::Get()->GetScene();
	if (!scene)
	{
		return;
	}

	if (ImGui::BeginTable("Data", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Hideable))
	{
		ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Data");

		int count = scene->m_EntitiesData->Count();
		for (int i = 0; i < count; i++)
		{
			const auto& entity = scene->m_EntitiesData->Get(i);

			std::string name = entity->GetName();

			ImGui::TableNextRow(ImGuiTableRowFlags_None);
			ImGui::TableNextColumn();

			ImGui::SetCursorPosX(5);

			ImGui::TextUnformatted(name.c_str());
		}

		ImGui::EndTable();
	}

	if (ImGui::BeginTable("Entities", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Hideable))
	{
		ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Entities");

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
}

void SceneHierarchyPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}
