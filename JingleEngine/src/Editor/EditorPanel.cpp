#include "Editor/EditorPanel.h"

#include "Editor/EditorModule.h"

BEGIN_CLASS_LINK(EditorPanel);
	LINK_CONSTRUCTOR();
	LINK_METHOD(OnBeginRender);
	LINK_METHOD(OnRender);
	LINK_METHOD(OnEndRender);
	LINK_METHOD(GetEditorModule);
	LINK_METHOD(Close);
END_CLASS_LINK();

EditorPanel::EditorPanel()
{
	m_Module = ModuleManager::Get<EditorModule>();
}

EditorPanel::~EditorPanel()
{
}

STATIC_FUNCTION(EditorPanel, OnBeginRender, void, double);
STATIC_FUNCTION(EditorPanel, OnRender, void, double);
STATIC_FUNCTION(EditorPanel, OnEndRender, void, double);

bool EditorPanel::Render(EditorPanel* panel, double DeltaTime, const std::string& title, ImGuiWindowFlags flags)
{
	std::string windowTitle = fmt::format("{}###{}", title, PointerToString(panel));

	bool isOpen = true;
	bool canClose = true;

	Script_OnBeginRender[panel](DeltaTime);

	bool* isOpenPtr = &isOpen;
	if ((flags & ImGuiWindowFlags_NoTitleBar) != 0)
	{
		isOpenPtr = nullptr;
	}
	
	if (ImGui::Begin(windowTitle.c_str(), isOpenPtr))
	{
		ImGui::PushID(panel);

		Script_OnRender[panel](DeltaTime);
		
		ImGui::PopID();
	}

	Script_OnEndRender[panel](DeltaTime);

	panel->m_ShouldClose |= !isOpen;
	if (panel->m_ShouldClose)
	{
		//! TODO: Handle logic that prevents closing such as "Unsaved data" prompt. Will set 'canClose' to 'false'
	}

	ImGui::End();

	return !(panel->m_ShouldClose && canClose);
}

void EditorPanel::OnBeginRender(double DeltaTime)
{
}

void EditorPanel::OnRender(double DeltaTime)
{
}

void EditorPanel::OnEndRender(double DeltaTime)
{
}

void EditorPanel::Close()
{
	m_ShouldClose = false;
}

