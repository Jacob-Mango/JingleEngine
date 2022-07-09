#pragma once

#include "Core/Core.h"

#include <imgui.h>
#include <imgui_internal.h>

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

namespace EditorUI
{
	struct PropertyRenderContext
	{
		uint64_t Depth = 0;

		float GetDepthIndent() const { return Depth * 4.0f; }
	};

	extern PropertyRenderContext Context;

	void ResetContext();

	void Render_CellHeader(std::string name, bool isHeader = false, bool isRightIndent = false);

	bool Render_CellInputText(std::string& value);

	int Render_SelectableInput(bool selected, ImGuiSelectableFlags selectFlags, ImGuiInputTextFlags inputFlags, std::string& text);

	template<typename T>
	static bool Render_Value(T& value, float width = -1.0f, const std::string& id = "")
	{
		const size_t BufferMaxSize = 256;
		char Buffer[BufferMaxSize] = "\0";

		ImGui::TableNextColumn();
		ImGui::PushItemWidth(width);
		std::string sValue = std::to_string(value);

		size_t size = std::min(sValue.size() + 1, BufferMaxSize);
		memset(Buffer, 0, BufferMaxSize);
		memcpy(Buffer, sValue.c_str(), size);

		std::string imguiId = "##" + PointerToString(&value) + id;

		if (ImGui::InputText(imguiId.c_str(), Buffer, size))
		{
			std::string buf = Buffer;
			std::istringstream ss(buf);
			ss >> value;
			
			return true;
		}

		return false;
	}

	template<typename T>
	static bool Render_Vector3(T& value)
	{
		ImGui::TableNextColumn();
		if (!ImGui::BeginTable("##vec3", 3, ImGuiTableFlags_None | ImGuiTableFlags_NoSavedSettings))
		{
			return false;
		}

		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
		ImGui::AlignTextToFramePadding();
		
		bool updated = false;

		ImGui::TableNextRow();
		updated |= Render_Value(value[0], contentRegionAvailable.x / 3.0f, "0");
		updated |= Render_Value(value[1], contentRegionAvailable.x / 3.0f, "1");
		updated |= Render_Value(value[2], contentRegionAvailable.x / 3.0f, "2");
		ImGui::EndTable();

		return updated;
	}
};
