#include "Editor/SceneHierarchyPanel.h"

#include "Core/Application.h"

#include <imgui.h>

BEGIN_CLASS_LINK(SceneHierarchyPanel);
END_CLASS_LINK();

void SceneHierarchyPanel::OnRender(double DeltaTime)
{
	if (!ImGui::BeginTable("test", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Hideable))
	{
		return;
	}

	ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
	ImGui::TableNextColumn();
	ImGui::TextUnformatted("Entities");

	Scene* scene = Application::Get()->GetScene();
	for (auto& entity : scene->m_Entities)
	{
		std::string name = entity->GetType()->Name();

		ImGui::TableNextRow(ImGuiTableRowFlags_None);
		ImGui::TableNextColumn();
		ImGui::TextUnformatted(name.c_str());
	}
	
	ImGui::EndTable();
}
