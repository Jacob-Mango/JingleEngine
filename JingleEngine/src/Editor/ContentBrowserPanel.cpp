#include "Editor/ContentBrowserPanel.h"

#include "Core/Application.h"

#include "Property/PropertyArray.h"

#include <imgui.h>
#include <imgui_internal.h>

BEGIN_CLASS_LINK(ContentBrowserPanel);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

ContentBrowserPanel::ContentBrowserPanel()
{
	m_RootPath = std::filesystem::current_path() / "Assets";
	m_CurrentDirectory = m_RootPath;
	m_PathChanged = true;
}

void ContentBrowserPanel::OnBeginRender(double DeltaTime)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

void ContentBrowserPanel::OnRender(double DeltaTime)
{
	//! Address bar
	{
		bool disabled = m_CurrentDirectory == m_RootPath;

		float width = ImGui::GetContentRegionAvail().x;
		float height = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
		float buttonWidth = 50.0f;

		if (disabled)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::Button("<", { buttonWidth, height }))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
			m_PathChanged = true;
		}

		if (disabled)
		{
			ImGui::EndDisabled();
		}

		ImGui::SameLine();

		auto relativePath = std::filesystem::relative(m_CurrentDirectory, m_RootPath).string();
		if (relativePath.compare(".") == 0) relativePath = "";

		const size_t bufferMaxSize = 256;
		size_t size = std::min(relativePath.size() + 1, bufferMaxSize);

		static char buffer[bufferMaxSize];

		if (m_PathChanged)
		{
			m_PathChanged = false;
			memset(buffer, 0, bufferMaxSize);
			memcpy(buffer, relativePath.c_str(), size);
		}

		ImGui::SetNextItemWidth(width - buttonWidth);

		if (ImGui::InputText("##ContentBrowserPath", buffer, size, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_PathChanged = true;

			std::filesystem::path newPath = m_RootPath / std::string(buffer);
			if (std::filesystem::exists(newPath))
			{
				m_CurrentDirectory = newPath;
			}
		}
	}

	// Browser
	{
		float columnWidth = 150.0f;
		float columnHeight = 30.0f;
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = std::max((int)(panelWidth / columnWidth), 1);

		ImGui::Columns(columnCount, 0, false);

		// Directories
		for (auto& file : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if (!file.is_directory())
			{
				continue;
			}

			auto path = file.path();
			AssetID id = std::filesystem::relative(path, m_RootPath).string();
			std::string fileName = path.filename().string();

			const char* fileNameCstr = fileName.c_str();
		
			ImGui::PushID(std::to_string(id.GetValue()).c_str());
			ImGui::ItemSize(ImRect(ImVec2(), ImVec2(columnWidth, columnHeight)));

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((columnWidth - ImGui::CalcTextSize(fileNameCstr).x) * 0.5f));
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ((columnHeight - ImGui::GetFontSize()) * 0.5f) - columnHeight);
			ImGui::TextWrapped(fileNameCstr);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				m_CurrentDirectory /= path.filename();
				m_PathChanged = true;
			}

			ImGui::PopID();
			ImGui::NextColumn();
		}

		// Files
		for (auto& file : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if (file.is_directory())
			{
				continue;
			}

			auto path = file.path();
			AssetID id = std::filesystem::relative(path, m_RootPath).string();
			std::string fileName = path.filename().string();

			const char* fileNameCstr = fileName.c_str();

			ImGui::PushID(std::to_string(id.GetValue()).c_str());
			ImGui::ItemSize(ImRect(ImVec2(), ImVec2(columnWidth, columnHeight)));

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((columnWidth - ImGui::CalcTextSize(fileNameCstr).x) * 0.5f));
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ((columnHeight - ImGui::GetFontSize()) * 0.5f) - columnHeight);
			ImGui::TextWrapped(fileNameCstr);

			ImGui::PopID();
			ImGui::NextColumn();
		}

		ImGui::Columns(1);
	}
}

void ContentBrowserPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar();
}
