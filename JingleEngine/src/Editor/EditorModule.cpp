#include "Editor/EditorModule.h"

#include "Core/Application.h"

#include "Editor/Editor.h"
#include "Editor/EditorAttribute.h"
#include "Editor/EditorPanel.h"

#include "Editor/Panels/ContentBrowserPanel.h"

#include <filesystem>

BEGIN_MODULE_LINK(EditorModule);
END_MODULE_LINK();

void EditorModule::OnInitialize()
{
	using namespace JingleScript;

	m_Editors.clear();

	Type* baseType = Editor::StaticType();

	for (auto& type : TypeManager::Iterator())
	{
		if (!type->IsInherited(baseType) || type == baseType)
		{
			continue;
		}

		EditorData data;

		for (auto& attributeBase : type->Attributes)
		{
			if (attributeBase->GetType() != EditorAttribute::StaticType())
			{
				continue;
			}

			data.Attribute = static_cast<EditorAttribute*>(attributeBase);
		}

		m_Editors.insert({ type, data });
	}
}

Editor* g_LastActiveEditor = nullptr;

bool EditorModule::PrepareRender()
{
	g_LastActiveEditor = nullptr;

	for (auto& [type, data] : m_Editors)
	{
		for (auto& editor : data.Instances)
		{
			std::string windowTitle = fmt::format("{}###{}", editor->GetFileName(), PointerToString(editor));

			ImGuiWindow* window = ImGui::FindWindowByName(windowTitle.c_str());
			if (!window)
			{
				g_LastActiveEditor = editor;
				continue;
			}

			if (window->DockNode->TabBar->SelectedTabId == window->TabId)
			{
				g_LastActiveEditor = editor;
			}
		}
	}

	return g_LastActiveEditor != nullptr;
}

void EditorModule::RenderMenu()
{
	if (ImGui::BeginMenu("Editor"))
	{
		ImGui::EndMenu();
	}

	if (g_LastActiveEditor)
	{
		g_LastActiveEditor->OnRenderMenu();
		return;
	}

}

ContentBrowserPanel* g_ContentBrowser = nullptr;

void EditorModule::RenderMain(double DeltaTime, ImGuiID DockspaceId)
{
	if (!g_ContentBrowser)
	{
		g_ContentBrowser = JingleScript::NewObject<ContentBrowserPanel>("ContentBrowserPanel");
	}

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;

	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImVec2 pos = viewport->WorkPos;
	ImVec2 size = viewport->WorkSize;

	ImGuiWindow* window = ImGui::FindWindowByName("JingleEngine Editor");
	if (window)
	{
		pos.y += window->MenuBarHeight();
		size.y -= window->MenuBarHeight();
	}

	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(size);
	ImGui::SetNextWindowViewport(viewport->ID);

	EditorPanel::Render(g_ContentBrowser, DeltaTime, "", DockspaceId, windowFlags);
}

void EditorModule::RenderEditors(double DeltaTime, ImGuiID DockspaceId)
{
	for (auto& [type, data] : m_Editors)
	{
		auto& instances = data.Instances;

		for (int i = instances.size() - 1; i >= 0; i--)
		{
			Editor* editor = instances[i];

			ImGui::PushID(editor);

			if (!editor->OnRender(DeltaTime, DockspaceId))
			{
				instances.erase(instances.begin() + i);
			}

			ImGui::PopID();
		}
	}
}

void EditorModule::OpenAsset(AssetID id)
{
	std::filesystem::path path = id.GetPath();

	if (path.extension().string() == ".ent")
	{
		Editor* editor = Open("EntityEditor");
		editor->Open(id);

		return;
	}
}

Editor* EditorModule::Open(std::string typeName)
{
	using namespace JingleScript;

	Type* type = TypeManager::Get(typeName);

	auto it = m_Editors.find(type);
	if (it == m_Editors.end())
	{
		return nullptr;
	}

	auto& data = it->second;

	Editor* editor = type->New<Editor>();
	data.Instances.push_back(editor);

	return editor;
}

void EditorModule::SetFileName(Editor* editor)
{
	using namespace JingleScript;

	if (!editor->m_AssetID.GetPath().empty())
	{
		std::filesystem::path p = editor->m_AssetID.GetPath();
		editor->m_FileName = p.filename().string();
		return;
	}

	Type* type = TypeManager::Get(editor->GetType()->Name());

	auto it = m_Editors.find(type);
	if (it == m_Editors.end())
	{
		editor->m_FileName = "Not Registered";
		return;
	}

	auto& data = it->second;

	editor->m_FileName = fmt::format("Unsaved {}", data.Instances.size());
}

void EditorModule::SelectEntity(Entity* entity)
{
	m_SelectedEntity = entity;
}

Entity* EditorModule::GetSelectedEntity()
{
	return m_SelectedEntity;
}

void EditorModule::ClearSelection()
{
	m_SelectedEntity = nullptr;
}
