#include "Editor/EntityPropertiesPanel.h"

#include "Core/Application.h"

#include "Property/PropertyArray.h"
#include "Property/PropertyAsset.h"
#include "Property/PropertyItem.h"
#include "Property/PropertyObject.h"

#include <imgui.h>

BEGIN_CLASS_LINK(EntityPropertiesPanel);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

void EntityPropertiesPanel::OnBeginRender(double DeltaTime)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

const float g_RowHeight = 30.0f;
int g_Depth = 0;
int g_LastX = 0;

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

	ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
	ImGui::AlignTextToFramePadding();

	auto render = [&](int index)
	{
		ImGui::TableNextColumn();

		ImGui::PushItemWidth(contentRegionAvailable.x / 3.0f);

		std::string str = std::to_string(vec[index]);

		strcpy_s(Buff, str.length() + 1, str.c_str());
		if (ImGui::InputText("", Buff, BuffSize))
		{
			std::stringstream ss;
			ss << Buff;
			ss >> vec[index];
		}
	};

	ImGui::TableNextRow();
	render(0);
	render(1);
	render(2);

	ImGui::EndTable();
}

void EntityPropertiesPanel::OnRender(double DeltaTime)
{
	Entity* entity = GetEditor()->GetSelectedEntity();
	if (entity == nullptr)
	{
		return;
	}

	g_Depth = 0;

	if (!ImGui::BeginTable("EntityPropertiesPanel", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{
		return;
	}

	g_LastX = ImGui::GetCursorPosX();

	for (auto& [name, property] : entity->GetPropertyContainer()->m_Properties)
	{
		if (name.compare("Children") == 0)
		{
			continue;
		}

		RenderPropertyBase(name, property);
	}

	ImGui::EndTable();
}

void EntityPropertiesPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}

void EntityPropertiesPanel::RenderPropertyArray(const std::string& name, PropertyArray* root)
{
	ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

	ImGui::TableNextColumn();
	ImGui::SetCursorPosX(g_Depth);
	ImGui::Text(name.c_str());

	int index = 0;
	for (auto& property : root->m_Properties)
	{
		std::string name = property->GetPropertyType()->Name() + " - " + std::to_string(index++);
		RenderPropertyBase(name, property);
	}
}

void EntityPropertiesPanel::RenderPropertyAsset(const std::string& name, PropertyAsset* root)
{
	ImGui::TableNextRow(0, g_RowHeight);

	ImGui::TableNextColumn();
	ImGui::SetCursorPosX(g_Depth);
	ImGui::Text(name.c_str());

	ImGui::TableNextColumn();
	ImGui::SetCursorPosX(g_LastX);
	ImGui::Text("asset");
}

void EntityPropertiesPanel::RenderPropertyBase(const std::string& name, PropertyBase* root)
{
	g_LastX = ImGui::GetCursorPosX();

	g_Depth+=4;
	ImGui::PushID(name.c_str());

	auto array = dynamic_cast<PropertyArray*>(root);
	auto asset = dynamic_cast<PropertyAsset*>(root);
	auto item = dynamic_cast<PropertyItem*>(root);
	auto object = dynamic_cast<PropertyObject*>(root);

	if (array)
	{
		RenderPropertyArray(name, array);
	}
	if (asset)
	{
		RenderPropertyAsset(name, asset);
	}
	if (item)
	{
		RenderPropertyItem(name, item);
	}
	if (object)
	{
		RenderPropertyObject(name, object);
	}

	ImGui::PopID();
	g_Depth-=4;
}

void EntityPropertiesPanel::RenderPropertyItem(const std::string& name, PropertyItem* root)
{
	ImGui::TableNextRow(0, g_RowHeight);

	ImGui::TableNextColumn();
	ImGui::SetCursorPosX(g_Depth);
	ImGui::Text(name.c_str());

	ImGui::TableNextColumn();
	ImGui::SetCursorPosX(g_LastX);
	ImGui::Text("item");
}

void EntityPropertiesPanel::RenderPropertyObject(const std::string& name, PropertyObject* root)
{
	ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

	ImGui::TableNextColumn();
	ImGui::SetCursorPosX(g_Depth);
	ImGui::Text(name.c_str());

	for (auto& [name, property] : root->m_Properties)
	{
		RenderPropertyBase(name, property);
	}
}
