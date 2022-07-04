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

};
