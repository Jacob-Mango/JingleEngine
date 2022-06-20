#include "Editor/EntityPropertiesPanel.h"

#include "Core/Application.h"

#include <imgui.h>

BEGIN_CLASS_LINK(EntityPropertiesPanel);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

void EntityPropertiesPanel::OnBeginRender(double DeltaTime)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

const float g_RowHeight = 30.0f;

void ImGui_Vec3(std::string name, glm::vec3& vec)
{
	static const int BuffSize = 128;
	static char Buff[BuffSize];

	ImGui::TableNextRow(0, g_RowHeight);
	ImGui::TableNextColumn();
	ImGui::Text(name.c_str());

	ImGui::TableNextColumn();
	if (!ImGui::BeginTable("ImGui_Vec3", 3, ImGuiTableFlags_None | ImGuiTableFlags_NoSavedSettings))
	{
		return;
	}

	//ImGui::GetStyle().

	//ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
	ImGui::AlignTextToFramePadding();

	auto render = [&](const char* title, int index)
	{
		ImGui::TableNextColumn();

		//ImGui::PushItemWidth(contentRegionAvailable.x / 6.0f);

		std::string str = std::to_string(vec[index]);

		strcpy_s(Buff, str.length() + 1, str.c_str());
		if (ImGui::InputText(title, Buff, BuffSize))
		{
			std::stringstream ss;
			ss << Buff;
			ss >> vec[index];
		}
	};

	ImGui::TableNextRow();
	render("X", 0);
	render("Y", 1);
	render("Z", 2);

	ImGui::EndTable();
}

void EntityPropertiesPanel::OnRender(double DeltaTime)
{
	Entity* entity = GetEditor()->GetSelectedEntity();
	if (entity == nullptr)
	{
	//	return;
	}

	static glm::vec3 pos;

	if (!ImGui::BeginTable("EntityPropertiesPanel", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{
		return;
	}

	ImGui::TableNextRow(0, g_RowHeight);
	ImGui::TableNextColumn();
	ImGui::Text("Advanced");
	ImGui::TableNextColumn();
	ImGui::Text("Advanced");

	ImGui_Vec3("Position", pos);

	ImGui::TableNextRow(0, g_RowHeight);
	ImGui::TableNextColumn();
	ImGui::Text("Advanced");
	ImGui::TableNextColumn();
	ImGui::Text("Advanced");

	ImGui::EndTable();
}

void EntityPropertiesPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}
