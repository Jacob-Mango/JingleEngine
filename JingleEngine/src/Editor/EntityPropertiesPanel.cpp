#include "Editor/EntityPropertiesPanel.h"

#include "Core/Application.h"

#include "Entities/Components/MeshComponent.h"

#include <imgui.h>

BEGIN_CLASS_LINK(EntityPropertiesPanel);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

void EntityPropertiesPanel::OnBeginRender(double DeltaTime)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

const float g_RowHeight = 30.0f;
const float g_DepthIndent = 4.0f;
int g_Depth = 0;

template<typename T>
class ScopedIncrement
{
	T* m_Value;

public:
	ScopedIncrement(T& value)
	{
		m_Value = &value;

		++(*m_Value);
	}

	~ScopedIncrement()
	{
		--(*m_Value);
	}
};

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
	m_ArrayIndex = 0;

	if (!ImGui::BeginTable("EntityPropertiesPanel", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{
		return;
	}

	ImGui::EndTable();
}

void EntityPropertiesPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}

void EntityPropertiesPanel::RenderPropertyHeader(const RenderContext& ctx, bool isHeader)
{
}

void EntityPropertiesPanel::RenderPropertyArray(const RenderContext& ctx, PropertyArray* root)
{
}

void EntityPropertiesPanel::RenderPropertyAsset(const RenderContext& ctx, PropertyAsset* root)
{
}

void EntityPropertiesPanel::RenderPropertyBase(const RenderContext& ctx, PropertyBase* root)
{
}

void EntityPropertiesPanel::RenderPropertyItem(const RenderContext& ctx, PropertyItem* root)
{
}

void EntityPropertiesPanel::RenderPropertyObject(const RenderContext& ctx, PropertyObject* root)
{
}
