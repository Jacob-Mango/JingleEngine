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

bool Render_Vec3(glm::vec3& value)
{
	static const int BuffSize = 128;
	static char Buff[BuffSize];
	
	ImGui::TableNextColumn();
	if (!ImGui::BeginTable("##vec3", 3, ImGuiTableFlags_None | ImGuiTableFlags_NoSavedSettings))
	{
		return false;
	}

	ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
	ImGui::AlignTextToFramePadding();
	auto render = [&](int index)
	{
		ImGui::TableNextColumn();
		ImGui::PushItemWidth(contentRegionAvailable.x / 3.0f);
		std::string str = std::to_string(value[index]);
		strcpy_s(Buff, str.length() + 1, str.c_str());
		if (ImGui::InputText(("##" + std::to_string(index)).c_str(), Buff, BuffSize))
		{
			std::stringstream ss;
			ss << Buff;
			ss >> value[index];

			return true;
		}

		return false;
	};

	bool updated = false;

	ImGui::TableNextRow();
	updated |= render(0);
	updated |= render(1);
	updated |= render(2);
	ImGui::EndTable();

	return updated;
}

};
