#include "Editor/EntityPropertiesPanel.h"

#include "Core/Application.h"

#include "Entities/Components/MeshComponent.h"

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

	for (auto& [name, property] : entity->GetPropertyContainer()->m_Properties)
	{
		if (name.compare("Children") == 0)
		{
			continue;
		}

		RenderContext ctx;
		ctx.m_Name = name;

		RenderPropertyBase(ctx, property);
	}

	ImGui::EndTable();
}

void EntityPropertiesPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}

void EntityPropertiesPanel::RenderPropertyHeader(const RenderContext& ctx, bool isHeader)
{
	if (!ctx.m_ShowHeader)
	{
		return;
	}

	ImGui::TableNextRow(isHeader ? ImGuiTableRowFlags_Headers : 0);

	ImGui::TableNextColumn();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (g_DepthIndent * g_Depth));
	ImGui::TextUnformatted(ctx.m_Name.c_str());
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - (g_DepthIndent * g_Depth));
}

void EntityPropertiesPanel::RenderPropertyArray(const RenderContext& ctx, PropertyArray* root)
{
	if (m_ArrayIndex >= MaxArrayDepth)
	{
		return;
	}

	ArrayContext& arrCtx = m_Arrays[m_ArrayIndex++];

	RenderPropertyHeader(ctx, true);

	ImGui::TableNextColumn();

	float width = ImGui::GetColumnWidth() * 0.5f;
	float height = ImGui::GetFrameHeight();

	bool addPressed = ImGui::Button("Add", { width, height });
	ImGui::SameLine();
	bool removePressed = ImGui::Button("Remove", { width, height });
	size_t count = root->m_Properties.size();
	size_t removeIndex = count;

	size_t index = 0;
	for (auto& property : root->m_Properties)
	{
		std::string name = "[" + std::to_string(index) + "] " + property->GetPropertyType()->Name();

		ImGui::TableNextRow();
		ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::GetColorU32(ImGuiCol_TableHeaderBg));

		ImGui::TableNextColumn();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (g_DepthIndent * g_Depth));
		bool selected = arrCtx.m_Selected == property;
		if (ImGui::Selectable(name.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns))
		{
			arrCtx.m_Selected = property;
		}

		if (selected)
		{
			removeIndex = index;
		}

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - (g_DepthIndent * g_Depth));

		{
			ScopedIncrement increment(g_Depth);

			RenderContext ctx;
			ctx.m_Name = name;
			ctx.m_ShowHeader = false;
			RenderPropertyBase(ctx, property);
		}

		index++;
	}

	//! TODO: Immediate reflection to the entity. Use 'GetWriteInstance' and pass 'instance' through the context

	if (addPressed)
	{
		JS_INFO("Add");

		PropertyObject* obj = JingleScript::NewObject<PropertyObject>("PropertyObject", MeshComponent::StaticType(), root->GetPropertyAttribute(), index);
		obj->Deserialize(nullptr, root);
		root->m_Properties.push_back(obj);
	}
	else if (removePressed && removeIndex != count)
	{
		JS_INFO("Remove");

		root->m_Properties.erase(root->m_Properties.begin() + removeIndex);
	}
}

void EntityPropertiesPanel::RenderPropertyAsset(const RenderContext& ctx, PropertyAsset* root)
{
	RenderPropertyHeader(ctx, false);

	ImGui::TableNextColumn();
	ImGui::TextUnformatted("asset");
}

void EntityPropertiesPanel::RenderPropertyBase(const RenderContext& ctx, PropertyBase* root)
{
	ScopedIncrement increment(g_Depth);

	ImGui::PushID(ctx.m_Name.c_str());

	auto array = dynamic_cast<PropertyArray*>(root);
	auto asset = dynamic_cast<PropertyAsset*>(root);
	auto item = dynamic_cast<PropertyItem*>(root);
	auto object = dynamic_cast<PropertyObject*>(root);

	if (array)
	{
		RenderPropertyArray(ctx, array);
	}

	if (asset)
	{
		RenderPropertyAsset(ctx, asset);
	}

	if (item)
	{
		RenderPropertyItem(ctx, item);
	}

	if (object)
	{
		RenderPropertyObject(ctx, object);
	}

	ImGui::PopID();
}

void EntityPropertiesPanel::RenderPropertyItem(const RenderContext& ctx, PropertyItem* root)
{
	RenderPropertyHeader(ctx, false);

	ImGui::TableNextColumn();
	ImGui::TextUnformatted("item");
}

void EntityPropertiesPanel::RenderPropertyObject(const RenderContext& ctx, PropertyObject* root)
{
	RenderPropertyHeader(ctx, true);

	for (auto& [name, property] : root->m_Properties)
	{
		RenderContext ctx;
		ctx.m_Name = name;
		RenderPropertyBase(ctx, property);
	}
}
