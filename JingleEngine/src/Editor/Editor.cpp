#include "Editor/Editor.h"

#include "Core/Application.h"

#include "Editor/EditorModule.h"

BEGIN_CLASS_LINK(Editor);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

Editor::Editor()
{
	m_Module = ModuleManager::Get<EditorModule>();

	using namespace JingleScript;

	m_Panels.clear();

	Type* baseType = EditorPanel::StaticType();

	for (auto& type : TypeManager::Iterator())
	{
		if (!type->IsInherited(baseType) || type == baseType)
		{
			continue;
		}

		EditorPanelData data;

		for (auto& attributeBase : type->Attributes)
		{
			if (attributeBase->GetType() != EditorAttribute::StaticType())
			{
				continue;
			}

			data.Attribute = static_cast<EditorAttribute*>(attributeBase);
		}

		m_Panels.insert({ type, data});
	}
}

Editor::~Editor()
{
}

bool Editor::OnRender(double DeltaTime, ImGuiID DockspaceId)
{
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
	ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	
	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::SetNextWindowDockID(DockspaceId, true ? ImGuiCond_Always : ImGuiCond_FirstUseEver);

	std::string windowTitle = fmt::format("{}###{}", GetFileName(), PointerToString(this));

	bool remainOpen = true;
	bool isOpen = ImGui::Begin(windowTitle.c_str(), &remainOpen, windowFlags);

	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	if (!remainOpen)
	{
		Close();
	}

	ImGuiID editorDockspaceId = ImGui::GetID(GetID().c_str());
	ImGui::DockSpace(editorDockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
	
	//! TODO: Add a popup here to cancel close if needed.
	
	for (auto& [type, data] : m_Panels)
	{
		auto& instances = data.Instances;

		for (int i = instances.size() - 1; i >= 0; i--)
		{
			EditorPanel* panel = instances[i];

			//ImGui::SetNextWindowDockID(editorDockspaceId, ImGuiCond_Always);
			ImGui::SetNextWindowDockID(editorDockspaceId, ImGuiCond_FirstUseEver);

			if (!EditorPanel::Render(panel, DeltaTime, data.Attribute->GetTitle(), 0))
			{
				//JingleScript::DeleteObject(instances[i]);
				
				instances.erase(instances.begin() + i);
			}
		}
	}

	ImGui::End();

	return remainOpen;
}

void Editor::OnRenderMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		bool canSave = !m_AssetID.GetPath().empty();

		if (ImGui::MenuItem("New", NULL))
		{
		}

		if (ImGui::MenuItem("Open", NULL))
		{
			//Open
		}

		if (ImGui::MenuItem("Save", NULL, false, canSave))
		{
			Save();
		}

		if (ImGui::MenuItem("Save As", NULL, false, canSave))
		{
		}

		if (ImGui::MenuItem("Close", NULL))
		{
			Close();
		}

		ImGui::EndMenu();
	}
	
	if (ImGui::BeginMenu("View"))
	{
		for (auto& [type, data] : m_Panels)
		{
			std::string title = data.Attribute->GetTitle();

			if (ImGui::MenuItem(title.c_str(), NULL))
			{
				OpenPanel(type->Name());
			}
		}

		ImGui::EndMenu();
	}
}

void Editor::Close()
{
	m_ShouldClose = false;
}

EditorPanel* Editor::OpenPanel(const std::string& typeName)
{
	using namespace JingleScript;

	Type* type = TypeManager::Get(typeName);

	auto it = m_Panels.find(type);
	if (it == m_Panels.end())
	{
		return nullptr;
	}

	auto& data = it->second;

	if (data.Instances.empty())
	{
		EditorPanel* panel = type->New<EditorPanel>();
		panel->m_Editor = this;
		data.Instances.push_back(panel);
	}

	return data.Instances[0];
}
