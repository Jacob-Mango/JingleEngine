#include "Editor/Editor.h"

namespace Editor
{

PropertyRenderContext Context;

void ResetContext()
{
	Context.Depth = 0;
}

void Render_CellHeader(std::string name, bool isHeader, bool isRightIndent)
{
	ImGui::TableNextRow(isHeader ? ImGuiTableRowFlags_Headers : 0);

	float indent = Context.GetDepthIndent();
	if (isRightIndent)
	{
		indent = ImGui::GetColumnWidth() - ImGui::CalcTextSize(name.c_str()).x;
	}

	ImGui::TableNextColumn();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + indent);
	ImGui::TextUnformatted(name.c_str());
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - indent);
}

bool Render_CellInputText(std::string& value)
{
	const size_t BufferMaxSize = 256;
	char Buffer[BufferMaxSize] = "\0";

	ImGui::TableNextColumn();

	size_t size = std::min(value.size() + 1, BufferMaxSize);
	memset(Buffer, 0, BufferMaxSize);
	memcpy(Buffer, value.c_str(), size);

	if (ImGui::InputText("##StructureProperty", Buffer, size, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		value = std::string(Buffer);
		return true;
	}

	return false;
}

//! Modified https://github.com/ocornut/imgui/issues/2718
int Render_SelectableInput(bool selected, ImGuiSelectableFlags selectFlags, ImGuiInputTextFlags inputFlags, std::string& text)
{
	using namespace ImGui;

	static const size_t BufferMaxSize = 256;
	static char Buffer[BufferMaxSize] = "\0";

	bool up_selected = false;
	bool up_typing = false;

	ImGuiContext& g = *ImGui::GetCurrentContext();
	ImGuiWindow* window = g.CurrentWindow;
	ImVec2 pos_before = window->DC.CursorPos;

	PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(g.Style.ItemSpacing.x, g.Style.FramePadding.y * 2.0f));
	up_selected = Selectable("##Selectable", selected, selectFlags | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowItemOverlap);
	PopStyleVar();

	ImGuiID id = window->GetID("##Input");
	bool temp_input_is_active = TempInputIsActive(id);
	bool temp_input_start = up_selected ? IsMouseDoubleClicked(0) : false;

	if (temp_input_start)
	{
		SetActiveID(id, window);
	}

	if (temp_input_is_active || temp_input_start)
	{
		ImVec2 pos_after = window->DC.CursorPos;
		window->DC.CursorPos = pos_before;
		up_typing = TempInputText(g.LastItemData.Rect, id, "##Input", Buffer, BufferMaxSize, inputFlags);
		if (up_typing)
		{
			text = std::string(Buffer);
		}

		window->DC.CursorPos = pos_after;
	}
	else
	{
		size_t size = std::min(text.size() + 1, BufferMaxSize);
		memset(Buffer, 0, BufferMaxSize);
		memcpy(Buffer, text.c_str(), size);
	}

	window->DrawList->AddText(pos_before, GetColorU32(ImGuiCol_Text), Buffer);

	return up_selected || up_typing;
}

};
