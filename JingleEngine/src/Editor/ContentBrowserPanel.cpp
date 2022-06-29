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
	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
}

void ContentBrowserPanel::OnRender(double DeltaTime)
{
	using namespace std::filesystem;

	if (ImGui::BeginTable("##table1", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
	{
		const float AddressBarHeight = 22.0f;

		//! Address bar
		ImGui::TableNextRow(ImGuiTableRowFlags_None, AddressBarHeight);
		ImGui::TableNextColumn();
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

		//! Browser
		
		float browserHeight = ImGui::GetContentRegionAvail().y - (ImGui::GetStyle().ItemSpacing.y * 2.0f);
		ImGui::TableNextRow(ImGuiTableRowFlags_None, browserHeight);
		ImGui::TableNextColumn();
		if (ImGui::BeginTable("##table2", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
		{
			ImGui::TableSetupColumn("##directory_browser", ImGuiTableColumnFlags_WidthStretch, 0.2f);

			ImGui::TableNextRow(ImGuiTableRowFlags_None, browserHeight);
			ImGui::TableNextColumn();

			std::function<void(const std::filesystem::path& path)> iterateDirectories;
			iterateDirectories = [&](const std::filesystem::path& path)
				{
					for (auto& file : std::filesystem::directory_iterator(path))
					{
						if (!file.is_directory())
						{
							continue;
						}

						auto path = file.path();
						std::string fileName = path.filename().string();

						size_t numFiles = std::count_if(directory_iterator(path), directory_iterator{}, [](const std::filesystem::directory_entry& file)
						{
							return file.is_directory();
						});

						ImGuiTreeNodeFlags flags = 0;
						flags |= numFiles == 0 ? ImGuiTreeNodeFlags_Leaf : 0;
						flags |= path == m_CurrentDirectory ? ImGuiTreeNodeFlags_Selected : 0;

						bool opened = ImGui::TreeNodeEx(fileName.c_str(), flags);
						if (ImGui::IsItemClicked())
						{
							m_CurrentDirectory = path;
							m_PathChanged = true;
						}

						if (opened)
						{
							ImGui::Indent();
							iterateDirectories(path);
							ImGui::Unindent();

							ImGui::TreePop();
						}
					}
				};

			if (ImGui::BeginTable("##table3", 1, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_ScrollY))
			{
				ImGui::TableNextRow(ImGuiTableRowFlags_None, browserHeight);
				ImGui::TableNextColumn();
				iterateDirectories(m_RootPath);
				ImGui::EndTable();
			}

			ImGui::TableNextColumn();

			float columnWidth = 150.0f;
			float columnHeight = 30.0f;
			float panelWidth = ImGui::GetColumnWidth();
			int columnCount = std::max((int)(panelWidth / columnWidth), 1);

			if (ImGui::BeginTable("##table4", columnCount, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_ScrollY))
			{
				int maxIndex = columnCount - 1;
				int index = maxIndex;

				auto renderFile = [&](const path& path, std::function<void(const std::filesystem::path&)> onDoubleClick)
					{
						if (index++ >= maxIndex)
						{
							ImGui::TableNextRow();
							index = 0;
						}

						ImGui::TableNextColumn();

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
							onDoubleClick(path);
						}

						ImGui::PopID();
					};

				// Directories
				for (auto& file : directory_iterator(m_CurrentDirectory))
				{
					if (!file.is_directory())
					{
						continue;
					}
					
					renderFile(file.path(), [&](const path& path)
						{
							m_CurrentDirectory /= path.filename();
							m_PathChanged = true;
						});
				}

				// Files
				for (auto& file : std::filesystem::directory_iterator(m_CurrentDirectory))
				{
					if (file.is_directory())
					{
						continue;
					}

					renderFile(file.path(), [&](const path& path)
						{
						});
				}

				ImGui::EndTable();
			};

			ImGui::EndTable();
		}

		ImGui::EndTable();
	}
}

void ContentBrowserPanel::OnEndRender(double DeltaTime)
{
	ImGui::PopStyleVar(2);
}
