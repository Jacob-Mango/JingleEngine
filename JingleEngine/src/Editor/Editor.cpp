#include "Editor/Editor.h"

BEGIN_CLASS_LINK(Editor);
	LINK_CONSTRUCTOR();
END_CLASS_LINK();

Editor::Editor()
{
	m_Editor = ModuleManager::Get<EditorModule>();
}

Editor::~Editor()
{
}

void Editor::OnRender(double DeltaTime)
{
	//ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
	//ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
	//
	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu("Options"))
	//	{
	//		if (ImGui::MenuItem("Close", NULL))
	//		{
	//			
	//		}
	//
	//		ImGui::EndMenu();
	//	}
	//
	//	auto editor = ModuleManager::Get<EditorModule>();
	//	if (editor)
	//	{
	//		editor->RenderMenuBar();
	//	}
	//
	//	ImGui::EndMenuBar();
	//}
}

void Editor::Close()
{
	m_ShouldClose = false;
}

