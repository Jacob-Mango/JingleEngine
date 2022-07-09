#include "Editor/EditorPanel.h"

#include "Editor/EditorModule.h"

BEGIN_CLASS_LINK(EditorPanel);
	LINK_CONSTRUCTOR();
	LINK_METHOD(OnBeginRender);
	LINK_METHOD(OnRender);
	LINK_METHOD(OnEndRender);
	LINK_METHOD(GetEditor);
	LINK_METHOD(Close);
END_CLASS_LINK();

EditorPanel::EditorPanel()
{
	m_Editor = ModuleManager::Get<EditorModule>();
}

EditorPanel::~EditorPanel()
{
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

void EditorPanel::OnEvent(BaseClass* sender, const EventArgs& args)
{
}

void EditorPanel::Close()
{
	m_ShouldClose = false;
}

